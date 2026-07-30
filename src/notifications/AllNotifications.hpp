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
#include "ClearErrorNotification.hpp"
#include "ModifySensorColourNotification.hpp"
#include "ModifySensorPositionNotification.hpp"
#include "ProjectSelectionCompleteNotification.hpp"

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

// Web-only notifications.

#include "web/CompleteProjectLoadNotification.hpp"

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_ALLNOTIFICATIONS_HPP
