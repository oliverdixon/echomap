//
// Created by owd on 25/06/2026.
//

#include "ProjectPanel.hpp"

#include <imgui.h>

#include "../objects/Project.hpp"
#include "../objects/Sensor.hpp"
#include "../objects/Signal.hpp"
#include "../services/IProjectObserveService.hpp"

namespace echomap
{

ProjectPanel::ProjectPanel(
        const IProjectObserveService& observer_service
) :
    panel_name(std::string("Project Explorer") + get_imgui_stable_name()),
    observer_service(observer_service)
{
}

void ProjectPanel::draw()
{
    constexpr ImGuiTreeNodeFlags default_flags = ImGuiTreeNodeFlags_DefaultOpen;

    if (ImGui::Begin(panel_name.c_str())) {
        if (observer_service.observe_project() == nullptr)
            ImGui::SeparatorText("No active project.");
        else {
            const auto& active_project = *observer_service.observe_project();
            ImGui::SeparatorText(active_project.get_c_str_name());
            if (ImGui::CollapsingHeader("Signals", default_flags))
                for (const auto& signal : active_project.observe_signals())
                    ImGui::TextUnformatted(signal.get_c_str_name());

            if (ImGui::CollapsingHeader("Sensors", default_flags))
                for (const auto& sensor : active_project.observe_sensors())
                    ImGui::TextUnformatted(sensor.get_c_str_name());

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

const char* ProjectPanel::get_imgui_stable_name() noexcept
{
    return "###ProjectPanel";
}

} // namespace echomap
