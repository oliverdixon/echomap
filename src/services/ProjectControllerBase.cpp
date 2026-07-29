/**
 * @file
 *
 * ProjectControllerBase implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#include "ProjectControllerBase.hpp"

#include <filesystem>

#include "../async/Worker.hpp"
#include "../async/tasks/LoadProjectTask.hpp"
#include "../notifications/AddChannelMappingNotification.hpp"
#include "../notifications/ModifySensorColourNotification.hpp"
#include "../notifications/ModifySensorPositionNotification.hpp"
#include "../objects/Project.hpp"
#include "../objects/Sensor.hpp"
#include "../utility/Logger.hpp"
#include "IProjectFilePicker.hpp"
#include "PanelHost.hpp"

namespace echomap
{

ProjectControllerBase::ProjectControllerBase(
        PanelHost& panel_host,
        std::unique_ptr<IProjectFilePicker> project_file_picker,
        Worker& worker
) :
    panel_host(panel_host),
    project_file_picker(std::move(project_file_picker)),
    worker(worker)
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

void ProjectControllerBase::request_open_project() const
{
    project_file_picker->request_project_file(
            [this](const std::filesystem::path& path) {
                worker.submit(std::make_unique<LoadProjectTask>(path, &worker));
            },
            {}
    );
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
