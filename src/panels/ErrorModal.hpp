/**
 * @file
 * @brief EchoMap modal error panel specification
 * @author Oliver Dixon
 * @date 2026-06-30
 */

#ifndef ECHOMAP_ERRORMODAL_HPP
#define ECHOMAP_ERRORMODAL_HPP

#include <sigc++/slot.h>

#include <string>

#include "IPanel.hpp"

namespace echomap
{

/**
 * Provides a persistent ImGui state to raise errors as a modal.
 *
 * Error modals provide a message, an optional detail string derived from an exception, and a single button to dismiss
 * the modal.
 */
class ErrorModal final : public IPanel
{
    using DismissedCallbackT = sigc::slot<void()>;

public:
    /**
     * Raise the modal given a simple unformatted message.
     *
     * @param message The unformatted string literal to display.
     * @param dismissed_callback Callback to invoke when the ErrorModal is cancelled.
     */
    explicit ErrorModal(
            std::string_view message,
            DismissedCallbackT&& dismissed_callback
    );

    /**
     * Raise the modal given a prefix and a detailed exception.
     *
     * @param new_prefix An unformatted string literal to display as the major text.
     * @param exception An exception containing a message to display as the minor text.
     * @param dismissed_callback Callback to invoke when the ErrorModal is cancelled.
     *
     * Since this routine will often be called from exception handlers, it provides a no-exception guarantee (to prevent
     * program termination). If a low-level system call fails, such as memory allocation to copy the error text, the
     * exception is caught and logged with the system logger; the modal is not raised.
     */
    ErrorModal(
            std::string_view new_prefix,
            const std::runtime_error& exception,
            DismissedCallbackT&& dismissed_callback
    );

    void draw() noexcept override;

    [[nodiscard]] const char* get_imgui_name() const noexcept override;

    void change_active_project(const Project* new_project) override;

    static const char* get_imgui_stable_name() noexcept;

private:
    std::string prefix = "An unknown error occurred.";
    std::optional<std::string> detail;
    std::string panel_name;
    DismissedCallbackT dismissed_callback;
};

} // namespace echomap

#endif // ECHOMAP_ERRORMODAL_HPP
