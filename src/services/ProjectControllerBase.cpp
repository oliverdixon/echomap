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
#include "../objects/Signal.hpp"
#include "../utility/Logger.hpp"
#include "PanelHost.hpp"

namespace echomap
{

ProjectControllerBase::ProjectControllerBase(
        PanelHost& panel_host
) :
    panel_host(panel_host)
{
}

ProjectControllerBase::~ProjectControllerBase() = default;

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

void ProjectControllerBase::handle_result(
        LoadSignalFileResult&& result
)
{
    if (project == nullptr || result.get_project_id() != project->get_id())
        LOG_F_WARN(
                "Dropping LoadSignalFileResult, which was intended for the unavailable Project with ID {}.",
                result.get_project_id()
        );
    else
        for (auto&& signals = std::move(result).take_signals(); auto signal : signals | std::views::as_rvalue)
            project->add_signal(std::move(signal));
}

} // namespace echomap
