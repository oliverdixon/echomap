/**
 * @file
 * @brief EchoMap sensor geometry panel implementation
 * @author Oliver Dixon
 * @date 2026-07-07
 */

#include "SensorGeometryPanel.hpp"

#include "../objects/Project.hpp"
#include "../objects/Sensor.hpp"
#include "../services/IProjectMutationService.hpp"
#include "../services/IProjectObserveService.hpp"
#include "../utility/Logger.hpp"

namespace echomap
{

SensorGeometryPanel::SensorGeometryPanel(
        IProjectMutationService& mutation_service,
        const IProjectObserveService& observer_service
) :
    panel_name(std::string("Sensor Geometry") + get_imgui_stable_name()),
    mutation_service(mutation_service),
    observer_service(observer_service)
{
}

const char* SensorGeometryPanel::get_imgui_name() const noexcept
{
    return panel_name.c_str();
}

void SensorGeometryPanel::draw()
{
    if (ImGui::Begin(panel_name.c_str())) {
        const auto* const optional_project = observer_service.observe_project();
        if (optional_project == nullptr)
            ImGui::Text("No project is loaded.");
        else if (optional_project->get_sensors_count() == 0u)
            ImGui::Text("No sensors are loaded.");
        else {
            recache_sensor_colours(*optional_project);
            draw_geometry_summary(*optional_project);
            draw_geometry_plot(*optional_project);
        }
    }

    ImGui::End();
}

const char* SensorGeometryPanel::get_imgui_stable_name() noexcept
{
    return "###SensorGeometryPanel";
}

void SensorGeometryPanel::recache_sensor_colours(
        const Project& active_project
) noexcept
{
    if (!cached_project_id.has_value() || *cached_project_id != active_project.get_id() ||
            active_project.get_sensors_count() != sensor_colours.size()) {
        // Ensure that we maintain the correct number of colours for the current number of sensors.
        try {
            sensor_colours.resize(active_project.get_sensors_count());
            for (auto [src, dst] : std::views::zip(active_project.observe_sensors(), sensor_colours))
                dst = IM_COL32(
                        static_cast<int>(src.colour.r * 255.0f),
                        static_cast<int>(src.colour.g * 255.0f),
                        static_cast<int>(src.colour.b * 255.0f),
                        static_cast<int>(src.colour.a * 255.0f)
                );
        } catch (const std::exception& exception) {
            LOG_F_ERROR("Could not store the sensor colouring: {}", exception.what());
            plotting_spec_3d.MarkerFillColors = nullptr;
            plotting_spec_3d.MarkerLineColors = nullptr;
            plotting_spec_3d.MarkerFillColor = ImVec4(255.0f, 255.0f, 255.0f, 255.0f);
            plotting_spec_3d.MarkerLineColor = IMPLOT_AUTO_COL;
            return;
        }

        // ... and update the plotting specification in case the resize invalidated pointers.
        plotting_spec_3d.MarkerFillColors = &*sensor_colours.begin();
        plotting_spec_3d.MarkerLineColors = plotting_spec_3d.MarkerFillColors;
        cached_project_id = active_project.get_id();
    }
}

void SensorGeometryPanel::draw_geometry_summary(
        const Project& active_project
) noexcept
{
    ImGui::SeparatorText("Geometry Summary");
    if (ImGui::BeginTable("##GeometrySummary", 5, table_flags)) {
        ImGui::TableSetupColumn("##SensorColourColumn", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Sensor", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("X Position", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Y Position", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Z Position", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        std::size_t row_idx = 0;

        for (const auto& sensor : active_project.observe_sensors()) {
            ImGui::PushID(static_cast<int>(sensor.get_id()));

            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            const ImVec4 colour = ImGui::ColorConvertU32ToFloat4(sensor_colours[row_idx]);
            std::array new_colour = {colour.x, colour.y, colour.z, colour.w};
            if (ImGui::ColorEdit4("##colour", new_colour.data(), ImGuiColorEditFlags_NoInputs)) {
                mutation_service.modify_sensor_colour(
                        sensor.get_id(),
                        {
                                .r = new_colour[0],
                                .g = new_colour[1],
                                .b = new_colour[2],
                                .a = new_colour[3],
                        }
                );

                sensor_colours[row_idx] = IM_COL32(
                        static_cast<int>(new_colour[0] * 255.0f),
                        static_cast<int>(new_colour[1] * 255.0f),
                        static_cast<int>(new_colour[2] * 255.0f),
                        static_cast<int>(new_colour[3] * 255.0f)
                );
            }

            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
            ImGui::TextUnformatted(sensor.get_c_str_name());

            Position new_position = sensor.position;
            bool position_changed = false;

            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
            position_changed = ImGui::InputFloat("##x", &new_position.x, 0.01f, 1.0f);

            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
            position_changed = ImGui::InputFloat("##y", &new_position.y, 0.01f, 1.0f) || position_changed;

            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
            position_changed = ImGui::InputFloat("##z", &new_position.z, 0.01f, 1.0f) || position_changed;

            ImGui::PopID();
            ++row_idx;

            if (position_changed)
                mutation_service.modify_sensor_position(sensor.get_id(), new_position);
        }

        ImGui::EndTable();
    }
}

void SensorGeometryPanel::draw_geometry_plot(
        const Project& active_project
) const noexcept
{
    ImGui::SeparatorText("Geometry Plot");
    ImPlot3D::PushStyleColor(ImPlot3DCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

    const auto avail_size = ImGui::GetContentRegionAvail();
    const float aspect_ratio = avail_size.x / avail_size.y;

    if (ImPlot3D::BeginPlot("##SensorGeometryPlot", ImVec2(-std::numeric_limits<float>::min(), avail_size.y))) {
        ImPlot3D::SetupBoxScale(aspect_ratio, 1.0f, 1.0f);
        ImPlot3D::SetupAxes("X", "Y", "Z");
        ImPlot3D::PlotScatterG(
                "",
                get_sensor_point,
                &active_project,
                static_cast<int>(active_project.get_sensors_count()),
                plotting_spec_3d
        );

        ImPlot3D::EndPlot();
    }

    ImPlot3D::PopStyleColor();
}

ImPlot3DPoint SensorGeometryPanel::get_sensor_point(
        const int idx,
        const void* const project_instance
) noexcept
{
    const auto* const project_ptr = static_cast<const Project*>(project_instance);

    const auto sensor_view = project_ptr->observe_sensors();
    assert(idx < sensor_view.size());

    /*
     * Performance note: observe_sensors returns a transform_view, which models random_access_range, and in turn
     * provides us with a random_access_iterator. These can be subscripted in constant time, so there is no real
     * performance penalty to using the externally accessible view instead of indexing the Project flat_map member
     * variable in a hot loop.
     */
    const auto [x, y, z] = sensor_view[idx].position;
    return {x, y, z};
}

} // namespace echomap
