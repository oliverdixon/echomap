/**
 * @file
 * @brief EchoMap modal error panel implementation
 * @author Oliver Dixon
 * @date 2026-06-30
 */

#include "ErrorModal.hpp"

#include <imgui.h>

#include "../utility/Logger.hpp"

namespace echomap
{

ErrorModal::ErrorModal(
        const std::string_view message,
        DismissedCallbackT&& dismissed_callback
) :
    panel_name(std::string("Error!") + get_imgui_stable_name()),
    dismissed_callback(std::move(dismissed_callback))
{
    try {
        prefix = message;
    } catch (const std::exception&) {
        LOG_ERROR("Could not allocate memory to display modal on UI.");
        return;
    }
}

ErrorModal::ErrorModal(
        const std::string_view new_prefix,
        const std::runtime_error& exception,
        DismissedCallbackT&& dismissed_callback
) :
    panel_name(std::string("Error!") + get_imgui_stable_name()),
    dismissed_callback(std::move(dismissed_callback))
{
    try {
        detail = exception.what();
        prefix = new_prefix;
    } catch (const std::exception&) {
        LOG_ERROR("Could not allocate memory to display modal on UI.");
        return;
    }
}

void ErrorModal::draw() noexcept
{
    ImGui::OpenPopup(get_imgui_name());
    if (ImGui::BeginPopupModal(get_imgui_name(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::TextUnformatted(prefix.c_str());
        ImGui::Separator();

        if (detail.has_value()) {
            ImGui::TextWrapped("%s", detail->c_str());
            ImGui::Separator();
        }

        if (ImGui::Button("Dismiss", button_size)) {
            ImGui::CloseCurrentPopup();
            dismissed_callback();
        }

        ImGui::EndPopup();
    }
}

const char* ErrorModal::get_imgui_name() const noexcept
{
    return panel_name.c_str();
}

void ErrorModal::change_active_project(
        const Project* const new_project
)
{
    std::ignore = new_project;
}

const char* ErrorModal::get_imgui_stable_name() noexcept
{
    return "###ErrorModal";
}

} // namespace echomap
