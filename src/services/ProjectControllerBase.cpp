/**
 * @file
 *
 * ProjectControllerBase implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#include "ProjectControllerBase.hpp"

#include "../async/results/LoadSignalFileResult.hpp"
#include "../notifications/AddChannelMappingNotification.hpp"
#include "../notifications/ModifySensorColourNotification.hpp"
#include "../notifications/ModifySensorPositionNotification.hpp"
#include "../objects/Project.hpp"
#include "../objects/Sensor.hpp"
#include "../utility/Logger.hpp"
#include "PanelHost.hpp"

#ifdef __EMSCRIPTEN__
#include "web/PartialProjectController.hpp"
#else
#include "native/FullProjectController.hpp"
#endif // __EMSCRIPTEN__

namespace echomap
{

ProjectControllerBase::ProjectControllerBase(
        PanelHost& panel_host
) :
    panel_host(panel_host)
{
}

ProjectControllerBase::~ProjectControllerBase() noexcept = default;

void ProjectControllerBase::change_active_project(
        std::unique_ptr<Project> new_project
)
{
    if (new_project == nullptr)
        LOG_DEBUG("Clearing the active project.");
    else
        LOG_F_DEBUG("Changing active project to {}.", new_project->get_name());

    project = std::move(new_project);
    panel_host.change_active_project(project.get());
}

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
