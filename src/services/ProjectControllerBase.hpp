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

#include "IProjectController.hpp"

namespace echomap
{

class PanelHost;
class Project;

/**
 * Base implementation for controllers requiring direct access to a Project, implementing the base set of Notification
 * and WorkerResult handlers.
 *
 * @todo Should be CRTP.
 */
class ProjectControllerBase : public IProjectController
{
public:
    explicit ProjectControllerBase(PanelHost& panel_host);
    ~ProjectControllerBase() override;

    ProjectControllerBase(const ProjectControllerBase&) = delete;
    ProjectControllerBase& operator=(const ProjectControllerBase&) = delete;

    ProjectControllerBase(ProjectControllerBase&&) = delete;
    ProjectControllerBase& operator=(ProjectControllerBase&&) = delete;

    void change_active_project(std::unique_ptr<Project> new_project) override;

    void handle_notification(const AddChannelMappingNotification& notification) const override;
    void handle_notification(const ModifySensorColourNotification& notification) const override;
    void handle_notification(const ModifySensorPositionNotification& notification) const override;

    void handle_result(LoadSignalFileResult&& result) override;

protected:
    PanelHost& panel_host;
    std::unique_ptr<Project> project; // NOLINT(*-non-private-member-variables-in-classes)
};

} // namespace echomap

#endif // ECHOMAP_PROJECTCONTROLLERBASE_HPP
