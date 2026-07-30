/**
 * @file
 *
 * FileChooser implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-22
 */

#if !defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "FileChooser.hpp"

namespace echomap
{

FileChooser::FileChooser(
        IRenderInvalidateService& invalidator,
        IFilePickerService::SuccessCallbackT&& success_callback,
        IFilePickerService::CancelledCallbackT&& cancelled_callback
) :
    file_combo(invalidator),
    panel_name(std::string("Select File to Open") + get_imgui_stable_name()),
    success_callback(std::move(success_callback)),
    cancelled_callback(std::move(cancelled_callback))
{
}

void FileChooser::draw() noexcept
{
    if (!std::exchange(is_open, true))
        ImGui::OpenPopup(get_imgui_name());

    ImGui::SetNextWindowSize(default_modal_size, ImGuiCond_Appearing);

    if (ImGui::BeginPopupModal(get_imgui_name(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::PushID(get_imgui_name());

        // Draw the file combo box.
        file_combo(chosen_path);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Button("Cancel", button_size)) {
            ImGui::CloseCurrentPopup();
            cancelled_callback();
        }

        ImGui::SameLine();

        if (chosen_path.extension() == expected_extension) {
            if (ImGui::Button("OK", button_size))
                success_callback(chosen_path);
        } else {
            ImGui::BeginDisabled();
            ImGui::Button("OK", button_size);
            ImGui::SetItemTooltip("To continue, select a JSON EchoMap project file.");
            ImGui::EndDisabled();
        }

        ImGui::PopID();
        ImGui::EndPopup();
    }
}

const char* FileChooser::get_imgui_name() const noexcept
{
    return panel_name.c_str();
}

const char* FileChooser::get_imgui_stable_name() noexcept
{
    return "###FileChooser";
}

} // namespace echomap

#endif // __EMSCRIPTEN__
