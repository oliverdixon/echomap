/**
 * @file
 *
 * IProjectController specification
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#ifndef ECHOMAP_IPROJECTCONTROLLER_HPP
#define ECHOMAP_IPROJECTCONTROLLER_HPP

namespace echomap
{

class Project;

struct ModifySensorPositionNotification;
struct ModifySensorColourNotification;
struct AddChannelMappingNotification;

class LoadSignalFileResult;
class LoadProjectResult;

/**
 * Interface for controllers requiring direct access to a Project or PartialProject.
 */
class IProjectController
{
public:
    IProjectController() = default;
    virtual ~IProjectController() = default;

    IProjectController(const IProjectController&) = default;
    IProjectController& operator=(const IProjectController&) = default;
    IProjectController(IProjectController&&) = default;
    IProjectController& operator=(IProjectController&&) = default;

    virtual void change_active_project(std::unique_ptr<Project> new_project) = 0;

    virtual void handle_notification(const AddChannelMappingNotification& notification) const = 0;
    virtual void handle_notification(const ModifySensorColourNotification& notification) const = 0;
    virtual void handle_notification(const ModifySensorPositionNotification& notification) const = 0;

    virtual void handle_result(LoadProjectResult&& result) = 0;
    virtual void handle_result(LoadSignalFileResult&& result) = 0;
};

} // namespace echomap

#endif // ECHOMAP_IPROJECTCONTROLLER_HPP
