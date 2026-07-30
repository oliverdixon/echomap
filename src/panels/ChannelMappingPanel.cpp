//
// Created by owd on 07/07/2026.
//

#include "ChannelMappingPanel.hpp"

#include "../objects/Project.hpp"
#include "../objects/Sensor.hpp"
#include "../objects/Signal.hpp"
#include "../services/IProjectMutationService.hpp"
#include "../services/IProjectObserveService.hpp"
#include "../services/IRenderInvalidator.hpp"

namespace echomap
{

ChannelMappingPanel::ChannelMappingPanel(
        IProjectMutationService& mutation_service,
        IRenderInvalidator& invalidator,
        const IProjectObserveService& observer_service
) :
    panel_name(std::string("Channel Mapping") + get_imgui_stable_name()),
    mutation_service(mutation_service),
    invalidator(invalidator),
    observer_service(observer_service)
{
}

const char* ChannelMappingPanel::get_imgui_name() const noexcept
{
    return panel_name.c_str();
}

void ChannelMappingPanel::draw() noexcept
{
    if (ImGui::Begin(panel_name.c_str())) {
        if (observer_service.observe_project() == nullptr)
            ImGui::Text("No project is loaded.");
        else {
            const auto& active_project = *observer_service.observe_project();
            ImGui::SeparatorText("Create Channel Mapping");
            draw_new_channel_mapping(active_project);

            // If a new mapping has been fully described, add it and prompt for another.
            if (new_entry_cache.signal != nullptr && new_entry_cache.sensor != nullptr) {
                mutation_service.add_channel_mapping(
                        new_entry_cache.signal->get_id(),
                        new_entry_cache.sensor->get_id()
                );

                new_entry_cache.signal = nullptr;
                new_entry_cache.sensor = nullptr;
            }

            ImGui::SeparatorText("Existing Channel Mapping");
            draw_existing_channel_mapping(active_project);
        }
    }

    ImGui::End();
}

const char* ChannelMappingPanel::get_imgui_stable_name() noexcept
{
    return "###ChannelMappingPanel";
}

void ChannelMappingPanel::draw_new_channel_mapping(
        const Project& active_project
) noexcept
{
    // TODO refactor monster.

    if (ImGui::BeginTable("##NewChannelMapping", 2, table_flags)) {
        ImGui::TableSetupColumn("Signal", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Sensor", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        // Prompt for the associated signal.
        ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
        const auto is_signal_combo_open = ImGui::BeginCombo(
                "##NewAssociationSignal",
                new_entry_cache.signal == nullptr ? "Select signal..." : new_entry_cache.signal->get_c_str_name(),
                0
        );

        if (is_signal_combo_open) {
            for (const auto& signal : active_project.observe_signals()) {
                const bool is_selected = new_entry_cache.signal == nullptr ? false : signal == *new_entry_cache.signal;

                // Checks if something has changed (thus current value needs updating).
                if (ImGui::Selectable(signal.get_c_str_name(), is_selected))
                    new_entry_cache.signal = &signal;

                // Checks if the current item is selected.
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        ImGui::TableNextColumn();

        // Prompt for the associated sensor.
        ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
        const auto is_sensor_combo_open = ImGui::BeginCombo(
                "##NewAssociationSensor",
                new_entry_cache.sensor == nullptr ? "Select sensor..." : new_entry_cache.sensor->get_c_str_name(),
                0
        );

        if (is_sensor_combo_open) {
            for (const auto& sensor : active_project.observe_sensors()) {
                const bool is_selected = new_entry_cache.sensor == nullptr ? false : sensor == *new_entry_cache.sensor;

                if (ImGui::Selectable(sensor.get_c_str_name(), is_selected))
                    new_entry_cache.sensor = &sensor;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        ImGui::EndTable();

        if ((is_signal_combo_open && !was_signal_combo_open) || (is_sensor_combo_open && !was_sensor_combo_open))
            invalidator.force_frames();

        was_signal_combo_open = is_signal_combo_open;
        was_sensor_combo_open = is_sensor_combo_open;
    }
}

void ChannelMappingPanel::draw_existing_channel_mapping(
        const Project& active_project
) noexcept
{
    if (ImGui::BeginTable("##ExistingChannelMapping", 2, table_flags)) {
        ImGui::TableSetupColumn("Signal", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Sensor", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        // Display existing associations.
        for (const auto& [signal, sensor] : active_project.observe_associations()) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
            ImGui::TextUnformatted(signal.get_c_str_name());
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
            ImGui::TextUnformatted(sensor.get_c_str_name());
        }

        ImGui::EndTable();
    }
}

} // namespace echomap
