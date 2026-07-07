//
// Created by owd on 07/07/2026.
//

#include "ChannelMappingPanel.hpp"

#include "../objects/Project.hpp"

namespace WebCFD
{

ChannelMappingPanel::ChannelMappingPanel(
        Project* const initial_project
) :
    active_project(initial_project)
{
}

const char* ChannelMappingPanel::get_imgui_name() const noexcept
{
    return panel_name.c_str();
}

void ChannelMappingPanel::draw() noexcept
{
    if (ImGui::Begin(panel_name.c_str())) {
        if (active_project == nullptr)
            ImGui::Text("No project is loaded.");
        else {
            ImGui::SeparatorText("Create Channel Mapping");
            draw_new_channel_mapping();

            // If a new mapping has been fully described, add it and prompt for another.
            if (new_mapping_cache.selected_signal != nullptr && new_mapping_cache.selected_sensor != nullptr) {
                try {
                    active_project->add_association(*new_mapping_cache.selected_signal, *new_mapping_cache.selected_sensor);
                } catch (const std::runtime_error& exception) {
                    error_modal.raise_error("Cannot add channel mapping.", exception);
                }

                new_mapping_cache.selected_signal = nullptr;
                new_mapping_cache.selected_sensor = nullptr;
            }

            ImGui::SeparatorText("Existing Channel Mapping");
            draw_existing_channel_mapping();
        }
    }

    error_modal.draw();
    ImGui::End();
}

void ChannelMappingPanel::set_active_project(
        Project* const new_active_project
) noexcept
{
    active_project = new_active_project;
}

void ChannelMappingPanel::draw_new_channel_mapping() noexcept
{
    if (ImGui::BeginTable("##NewChannelMapping", 2, table_flags)) {
        ImGui::TableSetupColumn("Signal", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Sensor", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        // Prompt for the associated signal.
        ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
        if (ImGui::BeginCombo(
                    "##NewAssociationSignal",
                    new_mapping_cache.selected_signal == nullptr ? "Select signal..."
                                                                 : new_mapping_cache.selected_signal->get_imgui_name(),
                    0
            )) {
            for (const auto& signal : active_project->observe_signals()) {
                const bool is_selected = new_mapping_cache.selected_signal == nullptr
                                                 ? false
                                                 : signal == *new_mapping_cache.selected_signal;

                // Checks if something has changed (thus current value needs updating).
                if (ImGui::Selectable(signal.get_imgui_name(), is_selected))
                    new_mapping_cache.selected_signal = &signal;

                // Checks if the current item is selected.
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        ImGui::TableNextColumn();

        // Prompt for the associated sensor.
        ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
        if (ImGui::BeginCombo(
                    "##NewAssociationSensor",
                    new_mapping_cache.selected_sensor == nullptr ? "Select sensor..."
                                                                 : new_mapping_cache.selected_sensor->get_imgui_name(),
                    0
            )) {
            for (const auto& sensor : active_project->observe_sensors()) {
                const bool is_selected = new_mapping_cache.selected_sensor == nullptr
                                                 ? false
                                                 : sensor == *new_mapping_cache.selected_sensor;

                if (ImGui::Selectable(sensor.get_imgui_name(), is_selected))
                    new_mapping_cache.selected_sensor = &sensor;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        ImGui::EndTable();
    }
}

void ChannelMappingPanel::draw_existing_channel_mapping() const noexcept
{
    if (ImGui::BeginTable("##ExistingChannelMapping", 2, table_flags)) {
        ImGui::TableSetupColumn("Signal", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Sensor", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        // Display existing associations.
        for (const auto& [signal, sensor] : active_project->observe_associations()) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
            ImGui::Text("%s", signal.get_imgui_name());
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
            ImGui::Text("%s", sensor.get_imgui_name());
        }

        ImGui::EndTable();
    }
}

} // namespace WebCFD
