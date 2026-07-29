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
#include "../utility/Logger.hpp"
#include "PanelHost.hpp"

#ifdef __EMSCRIPTEN__
#include "web/PartialProjectController.hpp"
#else
#include "native/FullProjectController.hpp"
#endif // __EMSCRIPTEN__

namespace echomap
{

template <class Derived>
ProjectControllerBase<Derived>::ProjectControllerBase(
        PanelHost& panel_host
) :
    panel_host(panel_host)
{
}

template <class Derived> ProjectControllerBase<Derived>::~ProjectControllerBase() noexcept = default;

template <class Derived>
void ProjectControllerBase<Derived>::change_active_project(
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

template <class Derived>
void ProjectControllerBase<Derived>::handle_notification(
        const AddChannelMappingNotification& notification
) const
{
    notification.verify_project(project.get());
    project->add_association(notification.signal_id, notification.sensor_id);
}

template <class Derived>
void ProjectControllerBase<Derived>::handle_notification(
        const ModifySensorColourNotification& notification
) const
{
    notification.verify_project(project.get());
    project->get_mutable_sensor(notification.sensor_id).set_colour(notification.colour);
}

template <class Derived>
void ProjectControllerBase<Derived>::handle_notification(
        const ModifySensorPositionNotification& notification
) const
{
    notification.verify_project(project.get());
    project->get_mutable_sensor(notification.sensor_id).set_position(notification.position);
}

template <class Derived>
void ProjectControllerBase<Derived>::handle_result(
        LoadProjectResult&& result
)
{
    static_cast<Derived*>(this)->handle_result_impl(std::move(result));
}

template <class Derived>
void ProjectControllerBase<Derived>::handle_result(
        LoadSignalFileResult&& result
)
{
    static_cast<Derived*>(this)->handle_result_impl(std::move(result));
}

#ifdef __EMSCRIPTEN__
template class ProjectControllerBase<PartialProjectController>;
#else
template class ProjectControllerBase<FullProjectController>;
#endif // __EMSCRIPTEN__

} // namespace echomap
