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

class Project;

/**
 * Base implementation for controllers requiring direct access to a Project, implementing the base set of Notification
 * and WorkerResult handlers.
 */
class ProjectControllerBase : public IProjectController
{
public:
    ProjectControllerBase();
    ~ProjectControllerBase() override;

    ProjectControllerBase(const ProjectControllerBase&) = delete;
    ProjectControllerBase& operator=(const ProjectControllerBase&) = delete;

    ProjectControllerBase(ProjectControllerBase&&) noexcept;
    ProjectControllerBase& operator=(ProjectControllerBase&&) noexcept;

    void handle_notification(const AddChannelMappingNotification& notification) const override;
    void handle_notification(const ModifySensorColourNotification& notification) const override;
    void handle_notification(const ModifySensorPositionNotification& notification) const override;

protected:
    std::unique_ptr<Project> project; // NOLINT(*-non-private-member-variables-in-classes)
};

} // namespace echomap

#endif // ECHOMAP_PROJECTCONTROLLERBASE_HPP
