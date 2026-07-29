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

#include "ProjectFilePicker.hpp"

namespace echomap
{

struct ModifySensorPositionNotification;
struct ModifySensorColourNotification;
struct AddChannelMappingNotification;

class PanelHost;
class Project;
class LoadSignalFileResult;
class LoadProjectResult;
class Worker;

/**
 * Base implementation for controllers requiring direct access to a Project, implementing the base set of Notification
 * and WorkerResult handlers.
 */
class ProjectControllerBase
{
public:
    explicit ProjectControllerBase(
            RenderHost& render_host,
            PanelHost& panel_host,
            Worker& worker
    );

    virtual ~ProjectControllerBase() noexcept;

    ProjectControllerBase(const ProjectControllerBase&) = delete;
    ProjectControllerBase& operator=(const ProjectControllerBase&) = delete;

    ProjectControllerBase(ProjectControllerBase&&) = delete;
    ProjectControllerBase& operator=(ProjectControllerBase&&) = delete;

    void change_active_project(std::unique_ptr<Project> new_project);

    void request_open_project();

    void handle_notification(const AddChannelMappingNotification& notification) const;
    void handle_notification(const ModifySensorColourNotification& notification) const;
    void handle_notification(const ModifySensorPositionNotification& notification) const;

    virtual void handle_result(LoadProjectResult&& result) = 0;
    virtual void handle_result(LoadSignalFileResult&& result) = 0;

protected:
    // NOLINTBEGIN(*-non-private-member-variables-in-classes)

    PanelHost& panel_host;
    ProjectFilePicker project_file_picker;
    std::unique_ptr<Project> project;
    Worker& worker;

    // NOLINTEND(*-non-private-member-variables-in-classes)
};

} // namespace echomap

#endif // ECHOMAP_PROJECTCONTROLLERBASE_HPP
