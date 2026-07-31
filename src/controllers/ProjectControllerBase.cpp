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
#include "../objects/Project.hpp"
#include "../objects/Sensor.hpp"
#include "../services/IFilePickerService.hpp"
#include "../utility/Logger.hpp"
#include "PanelHost.hpp"

namespace echomap
{

ProjectControllerBase::ProjectControllerBase(
        PanelHost& panel_host,
        std::unique_ptr<IFilePickerService> file_picker_service,
        Worker& worker
) :
    panel_host(panel_host),
    project_file_picker(std::move(file_picker_service)),
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
}

const Project* ProjectControllerBase::observe_project() const noexcept
{
    return project.get();
}

void ProjectControllerBase::request_open_project()
{
    project_file_picker->request_project_file(
            [this](const std::filesystem::path& path) {
                worker.submit(std::make_unique<LoadProjectTask>(path, &worker));
            },
            {}
    );
}

void ProjectControllerBase::add_channel_mapping(
        const Signal& signal,
        const Sensor& sensor
)
{
    if (project == nullptr)
        LOG_WARN("Dropping new channel mapping due to empty project.");
    else
        try {
            project->add_association(signal, sensor);
        } catch (const std::runtime_error& error) {
            panel_host.raise_error(error.what());
        }
}

void ProjectControllerBase::modify_sensor_position(
        const id_type sensor_id,
        const Position& position
)
{
    if (project == nullptr)
        LOG_WARN("Dropping Sensor position modification due to empty project.");
    else
        try {
            project->get_mutable_sensor(sensor_id).set_position(position);
        } catch (const std::runtime_error& error) {
            panel_host.raise_error(error.what());
        }
}

void ProjectControllerBase::modify_sensor_colour(
        const id_type sensor_id,
        const Colour& colour
)
{
    if (project == nullptr)
        LOG_WARN("Dropping Sensor colour modification due to empty project.");
    else
        try {
            project->get_mutable_sensor(sensor_id).set_colour(colour);
        } catch (const std::runtime_error& error) {
            panel_host.raise_error(error.what());
        }
}

} // namespace echomap
