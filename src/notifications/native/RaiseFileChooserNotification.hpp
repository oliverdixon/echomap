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

#ifndef __EMSCRIPTEN__

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
    explicit RaiseFileChooserNotification(
            sigc::slot<void(const std::filesystem::path&)>&& callback
    ) :
        callback(std::move(callback))
    {
    }

    sigc::slot<void(const std::filesystem::path&)> callback;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_RAISEFILECHOOSERNOTIFICATION_HPP
