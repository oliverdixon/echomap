/**
 * @file
 * @brief EchoMap class implementation
 * @author Oliver Dixon
 * @date 2026-05-05
 */

#include "EchoMap.hpp"

#include <imgui_impl_glfw.h>
#include <sigc++/adaptors/bind.h>

#include "async/tasks/LoadProjectTask.hpp"
#include "errors/IgnoredWarning.hpp"
#include "notifications/AllNotifications.hpp"
#include "objects/Project.hpp"
#include "objects/Sensor.hpp"
#include "objects/Signal.hpp"
#include "utility/Logger.hpp"

#ifdef __EMSCRIPTEN__
#include "objects/web/PartialProject.hpp"
#endif

namespace echomap
{

EchoMap::EchoMap() :
    worker{[] {
#if !defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)
        glfwPostEmptyEvent();
#endif
    }},
    panel_host(
            worker,
            despatcher,
            render_host
    )
{
    setup_subscriptions();
}

EchoMap::~EchoMap() noexcept = default;

void EchoMap::tick()
{
    process_notifications();
    process_worker_results();
}

void EchoMap::setup_subscriptions()
{
    // NOLINTBEGIN(*-redundant-casting) - False positive; casts are required for libsigcpp to resolve overloads.

    connections.emplace_back(
            despatcher.load_project_finished_channel.nominate_consumer(
            sigc::mem_fun(*this, static_cast<void (EchoMap::*)(LoadProjectResult&&)>(&EchoMap::handle_result))
    ));

    connections.emplace_back(
            despatcher.load_signal_file_channel.nominate_consumer(
            sigc::mem_fun(*this, static_cast<void (EchoMap::*)(LoadSignalFileResult&&)>(&EchoMap::handle_result))
    ));

    // NOLINTEND(*-redundant-casting)

    connections.emplace_back(despatcher.error_channel.observe([this](const ErrorResult& error) {
        panel_host.raise_error(error.what());
        LOG_F_ERROR("Error modal raised due to error: {}", error.what());
    }));
}

void EchoMap::process_notifications()
{
    while (!notification_queue.empty()) {
        auto notification = std::move(notification_queue.front());
        notification_queue.pop_front();

        const auto type_name = NotificationNames::indexed_names[notification.index()];
        auto* const hint = static_cast<void*>(&notification);

        LOG_F_DEBUG("Consuming {} with hint {}.", type_name, hint);

        try {
            visit_notification(std::move(notification));
        } catch (const IgnoredWarning& warning) {
            LOG_F_WARN("{} with hint {} was dropped: {}", type_name, hint, warning.what());
        } catch (const std::exception& exception) {
            panel_host.raise_error(exception.what());
            LOG_F_ERROR("{} with hint {} was responsible for error: {}", type_name, hint, exception.what());
        }
    }
}

void EchoMap::process_worker_results()
{
    while (auto result = worker.try_get_result())
        try {
            despatcher.publish(std::move(*result));
        } catch (const std::exception& exception) {
            Logger::log(Logger::Level::Error, exception.what(), std::source_location::current());
        }
}

void EchoMap::handle_notification(
        const ProjectSelectionCompleteNotification& notification
)
{
    panel_host.reset_active_modal();

    if (notification.path.has_value())
        worker.submit(std::make_unique<LoadProjectTask>(*notification.path, &worker));
}

void EchoMap::handle_notification(
        const ClearErrorNotification& notification
)
{
    std::ignore = notification;
    panel_host.clear_error();
}

void EchoMap::handle_result(
        LoadProjectResult&& result
)
{
    if (panel_host.is_modal_shown()) {
        LOG_WARN("Ignoring request to change active Project since there is an active modal.");
        return;
    }

    change_active_project(std::move(std::move(result).take_project()));
}

void EchoMap::handle_result(
        LoadSignalFileResult&& result
)
{
    if (project == nullptr || result.get_project_id() != project->get_id())
        LOG_F_WARN(
                "Dropping LoadSignalFileResult, which was intended for the unavailable Project with ID {}.",
                result.get_project_id()
        );
    else
        for (auto&& signals = std::move(result).take_signals(); auto signal : signals | std::views::as_rvalue)
            project->add_signal(std::move(signal));
}

void EchoMap::change_active_project(
        std::unique_ptr<Project> new_project
) noexcept
{
    if (new_project == nullptr)
        LOG_DEBUG("Clearing the active project.");
    else
        LOG_F_DEBUG("Changing active project to {}.", new_project->get_name());

    project = std::move(new_project);
    panel_host.change_active_project(project.get());
}

void EchoMap::notify(
        Notification&& notification
)
{
    notification_queue.emplace_back(std::move(notification));

    /*
     * The address is just a "hint" (as opposed to an ID) because the queue might be re-allocated. It's a best-guess
     * effort to quickly discriminate o notification without adding bloat to their structures.
     */
    LOG_F_DEBUG(
            "Scheduling {} with hint {} at position {}.",
            NotificationNames::indexed_names[notification_queue.back().index()],
            static_cast<void*>(&notification_queue.back()),
            notification_queue.size() - 1
    );
}

} // namespace echomap
