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
#include "../errors/IgnoredWarning.hpp"
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

void ProjectControllerBase::add_channel_mapping(
        const id_type signal_id,
        const id_type sensor_id
)
{
    if (project == nullptr)
        throw IgnoredWarning("Dropping new channel mapping due to empty project.");

    project->add_association(signal_id, sensor_id);
}

void ProjectControllerBase::modify_sensor_position(
        const id_type sensor_id,
        const Position& position
)
{
    if (project == nullptr)
        throw IgnoredWarning("Dropping Sensor position modification due to empty project.");

    project->get_mutable_sensor(sensor_id).set_position(position);
}

void ProjectControllerBase::modify_sensor_colour(
        const id_type sensor_id,
        const Colour& colour
)
{
    if (project == nullptr)
        throw IgnoredWarning("Dropping Sensor colour modification due to empty project.");

    project->get_mutable_sensor(sensor_id).set_colour(colour);
}

} // namespace echomap
