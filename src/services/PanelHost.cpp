/**
 * @file
 *
 * PanelHost implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-28
 */

#include "PanelHost.hpp"

#include <imgui_internal.h>

#include "../panels/ChannelMappingPanel.hpp"
#include "../panels/ErrorModal.hpp"
#include "../panels/IProjectPanel.hpp"
#include "../panels/MenuPanel.hpp"
#include "../panels/ProjectPanel.hpp"
#include "../panels/SensorGeometryPanel.hpp"
#include "../panels/SignalDFTPanel.hpp"
#include "../panels/SignalWaveformPanel.hpp"

namespace echomap
{

PanelHost::PanelHost(
        Worker& worker,
        WorkerResultDespatcher& despatcher,
        IRenderInvalidator& invalidator_service
)
{
    panels.push_back(std::make_unique<MenuPanel>());
    panels.push_back(std::make_unique<ProjectPanel>());
    panels.push_back(std::make_unique<SignalWaveformPanel>(&worker, despatcher));
    // panels.push_back(std::make_unique<SensorGeometryPanel>(this)); // TODO
    // panels.push_back(std::make_unique<ChannelMappingPanel>(this)); // TODO
    panels.push_back(std::make_unique<SignalDFTPanel>(&worker, despatcher, &invalidator_service));
}

PanelHost::~PanelHost() noexcept = default;

void PanelHost::draw_all() const
{
    // Draw the panels and express any applicable error state.
    for (const auto& panel : panels)
        panel->draw();

    if (active_modal != nullptr)
        active_modal->draw();

    if (error_modal != nullptr)
        error_modal->draw();
}

void PanelHost::reset_active_modal() noexcept
{
    active_modal.reset();
}

void PanelHost::change_active_modal(
        std::unique_ptr<IPanel> modal
) noexcept
{
    active_modal = std::move(modal);
}

void PanelHost::configure_default_dockspace(
        const ImGuiID dockspace_id
)
{
    // Left (narrow project explorer pane) and main workspace.
    ImGuiID dock_id_left = 0;
    ImGuiID dock_id_main = 0;
    ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, .15f, &dock_id_left, &dock_id_main);
    ImGui::DockBuilderDockWindow(ProjectPanel::get_imgui_stable_name(), dock_id_left);

    // Upper/lower
    ImGuiID dock_id_main_upper = 0;
    ImGuiID dock_id_main_lower = 0;
    ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Up, .33f, &dock_id_main_upper, &dock_id_main_lower);

    // Upper left/right
    ImGuiID dock_id_main_upper_left = 0;
    ImGuiID dock_id_main_upper_right = 0;
    ImGui::DockBuilderSplitNode(
            dock_id_main_upper,
            ImGuiDir_Left,
            .5f,
            &dock_id_main_upper_left,
            &dock_id_main_upper_right
    );
    ImGui::DockBuilderDockWindow(ChannelMappingPanel::get_imgui_stable_name(), dock_id_main_upper_left);
    ImGui::DockBuilderDockWindow(SensorGeometryPanel::get_imgui_stable_name(), dock_id_main_upper_right);

    // Lower left/right
    ImGuiID dock_id_main_lower_left = 0;
    ImGuiID dock_id_main_lower_right = 0;
    ImGui::DockBuilderSplitNode(
            dock_id_main_lower,
            ImGuiDir_Left,
            .5f,
            &dock_id_main_lower_left,
            &dock_id_main_lower_right
    );
    ImGui::DockBuilderDockWindow(SignalWaveformPanel::get_imgui_stable_name(), dock_id_main_lower_left);
    ImGui::DockBuilderDockWindow(SignalDFTPanel::get_imgui_stable_name(), dock_id_main_lower_right);
}

void PanelHost::change_active_project(
        const Project* const new_project
) const
{
    for (const auto& panel : panels)
        panel->change_active_project(new_project);
}

void PanelHost::raise_error(
        const std::string_view message
)
{
    error_modal = std::make_unique<ErrorModal>(message, sigc::mem_fun(*this, &PanelHost::clear_error));
}

void PanelHost::raise_error(
        const std::string_view message,
        const std::runtime_error& exception
)
{
    error_modal = std::make_unique<ErrorModal>(message, exception, sigc::mem_fun(*this, &PanelHost::clear_error));
}

void PanelHost::clear_error() noexcept
{
    error_modal.reset();
}

bool PanelHost::is_modal_shown() const noexcept
{
    return active_modal != nullptr || error_modal != nullptr;
}

} // namespace echomap
