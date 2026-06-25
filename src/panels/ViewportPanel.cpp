/**
 * @file
 * @brief WebCFD main viewport panel implementation
 * @author Oliver Dixon
 * @date 2026-06-20
 */

#include <imgui.h>
#include <implot.h>

#include "ViewportPanel.hpp"

namespace WebCFD
{

ViewportPanel::ViewportPanel()
{
    plotting_spec.Stride = sizeof(Signal::Sample);
}

ViewportPanel::ViewportPanel(
        Project * const project
) :
    active_project(project)
{
    plotting_spec.Stride = sizeof(Signal::Sample);
}

const char* ViewportPanel::get_imgui_name() const noexcept
{
    return panel_name.c_str();
}

void ViewportPanel::draw() noexcept
{
    if (ImGui::Begin(panel_name.c_str(), nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoTitleBar)) {
        if (ImGui::BeginTabBar("##ViewportTabBar")) {
            if (ImGui::BeginTabItem("Signal Waveforms")) {
                // TODO...
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Sensor Geometry")) {
                // TODO...
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Channel Mapping")) {
                if (ImGui::BeginTable("ChannelMappingTable", 3, ImGuiTableFlags_Borders)) {
                    ImGui::TableNextColumn();
                    ImGui::Text("Signal");
                    ImGui::TableNextColumn();
                    ImGui::Text("Sensor");
                    ImGui::TableNextColumn();
                    ImGui::Text("Mapping Status");

                    ImGui::EndTable();
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Localisation")) {
                // TODO...
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }

    ImGui::End();
}

} // namespace WebCFD
