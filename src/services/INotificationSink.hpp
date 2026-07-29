/**
 * @file
 *
 * INotificationSink specification
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#ifndef ECHOMAP_INOTIFICATIONSINK_HPP
#define ECHOMAP_INOTIFICATIONSINK_HPP

#include "../notifications/AllNotificationsFwd.hpp"

namespace echomap
{

/**
 * A service for accepting Notification objects.
 */
class INotificationSink
{
public:
    virtual ~INotificationSink() = default;

    INotificationSink(const INotificationSink&) = default;
    INotificationSink& operator=(const INotificationSink&) = default;
    INotificationSink(INotificationSink&&) noexcept = default;
    INotificationSink& operator=(INotificationSink&&) noexcept = default;

    virtual void notify(Notification&& notification) = 0;
};

} // namespace echomap

#endif // ECHOMAP_INOTIFICATIONSINK_HPP
