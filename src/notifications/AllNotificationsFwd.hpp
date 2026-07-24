/**
 * @file
 *
 * AllNotificationsFwd specification
 *
 * @author Oliver Dixon
 * @date 2026-07-24
 */

#ifndef ECHOMAP_ALLNOTIFICATIONSFWD_HPP
#define ECHOMAP_ALLNOTIFICATIONSFWD_HPP

#include "../utility/VariantHelpers.hpp"

namespace echomap
{

struct AddChannelMappingNotification;
struct ModifySensorColourNotification;
struct ModifySensorPositionNotification;
struct ProjectSelectedNotification;
struct CompleteProjectLoadNotification;
struct RegisterVFSMappingNotification;
struct CancelProjectLoadNotification;

#ifndef __EMSCRIPTEN__

// Native-only notifications.

struct RaiseFileChooserNotification;

#endif // __EMSCRIPTEN__

/**
 * @defgroup Notifications Notifications
 * A trivial message sent exclusively to the EchoMap controller.
 * @todo Add relevant member functions
 */

/**
 * A Notification is a trivial message sent exclusively to the EchoMap controller.
 *
 * @ingroup Notifications
 */
using Notification = std::variant<
        AddChannelMappingNotification,
        ModifySensorColourNotification,
        ModifySensorPositionNotification,
        ProjectSelectedNotification,
        CompleteProjectLoadNotification,
        RegisterVFSMappingNotification,
        CancelProjectLoadNotification
#ifndef __EMSCRIPTEN__
        // Native-only notifications.
        ,
        RaiseFileChooserNotification
#endif // __EMSCRIPTEN__
        >;

/**
 * Helper to retrieve names of Notification variant alternatives.
 *
 * @ingroup Notifications
 */
struct NotificationNames
{
    /**
     * Retrieve a human-readable name for the templated Notification.
     *
     * @return A NULL-terminated static non-owning view of the name.
     */
    template <typename NotificationT>
        requires variant_helpers::VariantAlternative<
                NotificationT,
                Notification>
    static constexpr std::string_view get()
    {
        // ReSharper disable once CppStaticAssertFailure
        static_assert(false, "Missing notification name.");
        return {};
    }
};

template <> constexpr std::string_view NotificationNames::get<AddChannelMappingNotification>()
{
    return "Add Channel Mapping Notification";
}

template <> constexpr std::string_view NotificationNames::get<CancelProjectLoadNotification>()
{
    return "Cancel Project Load Notification";
}

template <> constexpr std::string_view NotificationNames::get<CompleteProjectLoadNotification>()
{
    return "Complete Project Load Notification";
}

template <> constexpr std::string_view NotificationNames::get<ModifySensorColourNotification>()
{
    return "Modify Sensor Colour Notification";
}

template <> constexpr std::string_view NotificationNames::get<ModifySensorPositionNotification>()
{
    return "Modify Sensor Position Notification";
}

template <> constexpr std::string_view NotificationNames::get<ProjectSelectedNotification>()
{
    return "Project Selected Notification";
}

template <> constexpr std::string_view NotificationNames::get<RegisterVFSMappingNotification>()
{
    return "Register VFS Mapping";
}

#ifndef __EMSCRIPTEN__

// Names of native-only notifications.

template <> constexpr std::string_view NotificationNames::get<RaiseFileChooserNotification>()
{
    return "Raise File Chooser Notification";
}

#endif // __EMSCRIPTEN__

} // namespace echomap

#endif // ECHOMAP_ALLNOTIFICATIONSFWD_HPP
