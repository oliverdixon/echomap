/**
 * @file
 *
 * AllNotifications specification
 *
 * @author Oliver Dixon
 * @date 2026-07-22
 */

#ifndef ECHOMAP_ALLNOTIFICATIONS_HPP
#define ECHOMAP_ALLNOTIFICATIONS_HPP

#include "AddChannelMappingNotification.hpp"
#include "AllNotificationsFwd.hpp"
#include "CancelProjectLoadNotification.hpp"
#include "CompleteProjectLoadNotification.hpp"
#include "ModifySensorColourNotification.hpp"
#include "ModifySensorPositionNotification.hpp"
#include "ProjectSelectedNotification.hpp"
#include "RegisterVFSMappingNotification.hpp"
#include "ClearErrorNotification.hpp"

#if !defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

// Native-only notifications.

#include "native/RaiseFileChooserNotification.hpp"

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_ALLNOTIFICATIONS_HPP
