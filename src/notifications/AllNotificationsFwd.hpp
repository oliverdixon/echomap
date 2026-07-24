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
struct ClearErrorNotification;

#if !defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

// Native-only notifications.

struct RaiseFileChooserNotification;

#endif // __EMSCRIPTEN__

/**
 * @defgroup Notifications Notifications
 * A trivial message sent exclusively to the EchoMap controller.
 */

/**
 * A Notification is a trivial message sent exclusively to the EchoMap controller.
 *
 * To add a new Notification:
 *  -# Create the struct in a file and add to the CMake DummyTarget source vector;
 *  -# Add the struct type to the forward declarations in this file;
 *  -# Add the forward-declared type to this variant;
 *  -# Specialise the NotificationNames::get member function to produce a human-readable name;
 *  -# Include the full type in AllNotifications.hpp;
 *  -# Add a handler for its messages in an EchoMap derived class;
 *  -# Add the handler function to any visitors; platform-independent handlers go in
 *      @ref EchoMap::make_common_notification_visitors.
 *
 * Once the type is added to this variant, all further steps are verified statically.
 *
 * If the Notification is applicable only to a certain platform, its symbols should not be included in translation units
 * compiled for anything except the target platform. The mechanism for managing this split is:
 *  - Separate CMake source vectors for platform-dependent translation units;
 *  - Separate inheritors of EchoMap for each platform (e.g. EchoMapNative, EchoMapWeb);
 *  - Where absolutely necessary, inline macro checks e.g. @c __EMSCRIPTEN__
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
        CancelProjectLoadNotification,
        ClearErrorNotification
#if !defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)
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

    /**
     * Aggregated names by the index of the alternative.
     */
    static const std::array<std::string_view, std::variant_size_v<Notification>> indexed_names;
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
    return "Register VFS Mapping Notification";
}

template <> constexpr std::string_view NotificationNames::get<ClearErrorNotification>()
{
    return "Clear Error State Notification";
}

#if !defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

// Names of native-only notifications.

template <> constexpr std::string_view NotificationNames::get<RaiseFileChooserNotification>()
{
    return "Raise File Chooser Notification";
}

#endif // __EMSCRIPTEN__

constexpr std::array<std::string_view, std::variant_size_v<Notification>> NotificationNames::indexed_names =
        variant_helpers::variant_name_array<Notification, NotificationNames>;

} // namespace echomap

#endif // ECHOMAP_ALLNOTIFICATIONSFWD_HPP
