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

namespace echomap
{

struct ModifySensorPositionNotification;
struct ModifySensorColourNotification;
struct AddChannelMappingNotification;

class IProjectFilePicker;
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
            PanelHost& panel_host,
            std::unique_ptr<IProjectFilePicker> project_file_picker,
            Worker& worker
    );

    virtual ~ProjectControllerBase() noexcept;

    ProjectControllerBase(const ProjectControllerBase&) = delete;
    ProjectControllerBase& operator=(const ProjectControllerBase&) = delete;

    ProjectControllerBase(ProjectControllerBase&&) = delete;
    ProjectControllerBase& operator=(ProjectControllerBase&&) = delete;

    void change_active_project(std::unique_ptr<Project> new_project);

    void request_open_project() const;

    void handle_notification(const AddChannelMappingNotification& notification) const;
    void handle_notification(const ModifySensorColourNotification& notification) const;
    void handle_notification(const ModifySensorPositionNotification& notification) const;

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
