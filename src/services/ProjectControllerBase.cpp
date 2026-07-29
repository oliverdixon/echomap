/**
 * @file
 *
 * ProjectControllerBase implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#include "ProjectControllerBase.hpp"

#include "../notifications/AddChannelMappingNotification.hpp"
#include "../notifications/ModifySensorColourNotification.hpp"
#include "../notifications/ModifySensorPositionNotification.hpp"
#include "../objects/Project.hpp"
#include "../objects/Sensor.hpp"

namespace echomap
{

ProjectControllerBase::ProjectControllerBase() = default;

ProjectControllerBase::~ProjectControllerBase() = default;

ProjectControllerBase::ProjectControllerBase(ProjectControllerBase&&) noexcept = default;

ProjectControllerBase& ProjectControllerBase::operator=(ProjectControllerBase&&) noexcept = default;

void ProjectControllerBase::handle_notification(
        const AddChannelMappingNotification& notification
) const
{
    notification.verify_project(project.get());
    project->add_association(notification.signal_id, notification.sensor_id);
}

void ProjectControllerBase::handle_notification(
        const ModifySensorColourNotification& notification
) const
{
    notification.verify_project(project.get());
    project->get_mutable_sensor(notification.sensor_id).set_colour(notification.colour);
}

void ProjectControllerBase::handle_notification(
        const ModifySensorPositionNotification& notification
) const
{
    notification.verify_project(project.get());
    project->get_mutable_sensor(notification.sensor_id).set_position(notification.position);
}

} // namespace echomap
