//
// Created by owd on 25/06/2026.
//

#include "ProjectPanel.hpp"

#include <imgui.h>

#include "../objects/Project.hpp"
#include "../objects/Sensor.hpp"
#include "../objects/Signal.hpp"

namespace echomap
{

ProjectPanel::ProjectPanel(
        const Project* const initial_project
) :
    panel_name(std::string("Project Explorer") + get_imgui_stable_name()),
    active_project(initial_project)
{
}

void ProjectPanel::draw() noexcept
{
    constexpr ImGuiTreeNodeFlags default_flags = ImGuiTreeNodeFlags_DefaultOpen;

    if (ImGui::Begin(panel_name.c_str())) {
        if (active_project == nullptr)
            ImGui::SeparatorText("No active project.");
        else {
            ImGui::SeparatorText(active_project->get_imgui_name());
            if (ImGui::CollapsingHeader("Signals", default_flags))
                for (const auto& signal : active_project->observe_signals())
                    ImGui::TextUnformatted(signal.get_imgui_name());

            if (ImGui::CollapsingHeader("Sensors", default_flags))
                for (const auto& sensor : active_project->observe_sensors())
                    ImGui::TextUnformatted(sensor.get_imgui_name());

            if (ImGui::CollapsingHeader("Results", default_flags)) {
                // TODO...
            }
        }
    }

    ImGui::End();
}

const char* ProjectPanel::get_imgui_name() const noexcept
{
    return panel_name.c_str();
}

void ProjectPanel::change_active_project(
        const Project* const new_project
)
{
    active_project = new_project;
}

const char* ProjectPanel::get_imgui_stable_name() noexcept
{
    return "###ProjectPanel";
}

} // namespace echomap
