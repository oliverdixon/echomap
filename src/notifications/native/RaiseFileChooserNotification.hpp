/**
 * @file
 *
 * RaiseFileChooserNotification specification
 *
 * @author Oliver Dixon
 * @date 2026-07-24
 */

#ifndef ECHOMAP_RAISEFILECHOOSERNOTIFICATION_HPP
#define ECHOMAP_RAISEFILECHOOSERNOTIFICATION_HPP

#if !defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include <sigc++/slot.h>

#include <filesystem>

namespace echomap
{

/**
 * A notification indicating that the NativeFileChooser should be raised to prompt for a new Project.
 *
 * Produced by the ActionController on ProjectFileAction invocation.
 *
 * @ingroup Notifications ProjectFileAction
 */
struct RaiseFileChooserNotification
{
    using SuccessCallbackT = sigc::slot<void(const std::filesystem::path&)>;
    using CancelledCallbackT = sigc::slot<void()>;

    /**
     * Create a new RaiseFileChooserNotification to indicate that the FileChooser should be raised.
     *
     * @param success_callback The callback for FileChooser to invoke once a file has been selected.
     * @param cancelled_callback The callback for FileChooser to invoke if it is dismissed prior to selection.
     */
    explicit RaiseFileChooserNotification(
            SuccessCallbackT&& success_callback,
            CancelledCallbackT&& cancelled_callback
    ) :
        success_callback(std::move(success_callback)),
        cancelled_callback(std::move(cancelled_callback))
    {
    }

    SuccessCallbackT success_callback;
    CancelledCallbackT cancelled_callback;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_RAISEFILECHOOSERNOTIFICATION_HPP
