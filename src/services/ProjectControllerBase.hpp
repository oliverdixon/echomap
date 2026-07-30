/**
 * @file
 *
 * ProjectControllerBase specification
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#ifndef ECHOMAP_PROJECTCONTROLLERBASE_HPP
#define ECHOMAP_PROJECTCONTROLLERBASE_HPP

#include <memory>

#include "IProjectMutationService.hpp"
#include "IProjectObserveService.hpp"
#include "IProjectOpenService.hpp"

namespace echomap
{

class IProjectFilePicker;
class PanelHost;
class Project;
class LoadSignalFileResult;
class LoadProjectResult;
class Worker;

/**
 * Base implementation for controllers requiring direct access to a Project, implementing the base set of WorkerResult
 * handlers.
 */
class ProjectControllerBase : public IProjectMutationService, public IProjectOpenService, public IProjectObserveService
{
public:
    explicit ProjectControllerBase(
            PanelHost& panel_host,
            std::unique_ptr<IProjectFilePicker> project_file_picker,
            Worker& worker
    );

    ~ProjectControllerBase() noexcept override;

    ProjectControllerBase(const ProjectControllerBase&) = delete;
    ProjectControllerBase& operator=(const ProjectControllerBase&) = delete;

    ProjectControllerBase(ProjectControllerBase&&) = delete;
    ProjectControllerBase& operator=(ProjectControllerBase&&) = delete;

    void change_active_project(std::unique_ptr<Project> new_project);
    [[nodiscard]] const Project* observe_project() const noexcept override;

    void request_open_project() override;

    void add_channel_mapping(
            id_type signal_id,
            id_type sensor_id
    ) override;

    void modify_sensor_position(
            id_type sensor_id,
            const Position& position
    ) override;

    void modify_sensor_colour(
            id_type sensor_id,
            const Colour& colour
    ) override;

    virtual void handle_result(LoadProjectResult&& result) = 0;
    virtual void handle_result(LoadSignalFileResult&& result) = 0;

protected:
    // NOLINTBEGIN(*-non-private-member-variables-in-classes)

    PanelHost& panel_host;
    std::unique_ptr<Project> project;
    std::unique_ptr<IProjectFilePicker> project_file_picker;
    Worker& worker;

    // NOLINTEND(*-non-private-member-variables-in-classes)
};

} // namespace echomap

#endif // ECHOMAP_PROJECTCONTROLLERBASE_HPP
