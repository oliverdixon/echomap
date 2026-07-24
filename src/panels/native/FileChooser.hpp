/**
 * @file
 *
 * FileChooser specification
 *
 * @author Oliver Dixon
 * @date 2026-07-22
 */

#ifndef ECHOMAP_FILECHOOSER_HPP
#define ECHOMAP_FILECHOOSER_HPP

#if !defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include <sigc++/slot.h>

#include <filesystem>

#include "../IPanel.hpp"
#include "FilesystemCombo.hpp"
#include "../../notifications/native/RaiseFileChooserNotification.hpp"

namespace echomap
{

class EchoMap;

/**
 * Modal containing controls for interactively selecting a file from the file-system.
 */
class FileChooser : public IPanel
{
public:
    /**
     * Create a new FileChooser modal for the given EchoMap instance.
     *
     * @param app The owning EchoMap instance.
     * @param success_callback The slot to invoke with the path once a file has been selected.
     * @param cancelled_callback The slot to invoke if the operation is cancelled.
     */
    explicit FileChooser(
            EchoMap* app,
            RaiseFileChooserNotification::SuccessCallbackT&& success_callback,
            RaiseFileChooserNotification::CancelledCallbackT&& cancelled_callback
    );

    void draw() noexcept override;

    [[nodiscard]] const char* get_imgui_name() const noexcept override;

    void change_active_project(const Project* new_project) override;

    static const char* get_imgui_stable_name() noexcept;

private:
    constexpr static ImVec2 default_modal_size{500.0f, 0.0f};

    std::filesystem::path expected_extension = ".json";
    std::filesystem::path chosen_path;
    FilesystemCombo file_combo;

    std::string panel_name;
    bool is_open = false;
    EchoMap* app;
    RaiseFileChooserNotification::SuccessCallbackT success_callback;
    RaiseFileChooserNotification::CancelledCallbackT cancelled_callback;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_FILECHOOSER_HPP
