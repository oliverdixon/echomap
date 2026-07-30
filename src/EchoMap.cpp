/**
 * @file
 * @brief EchoMap class implementation
 * @author Oliver Dixon
 * @date 2026-05-05
 */

#include "EchoMap.hpp"

#include <sigc++/adaptors/bind.h>

#include "async/tasks/LoadProjectTask.hpp"
#include "errors/IgnoredWarning.hpp"
#include "notifications/AllNotifications.hpp"
#include "objects/Project.hpp"
#include "objects/Signal.hpp"
#include "panels/MenuPanel.hpp"
#include "panels/ProjectPanel.hpp"
#include "panels/SignalDFTPanel.hpp"
#include "panels/SignalWaveformPanel.hpp"
#include "utility/Logger.hpp"

namespace echomap
{

EchoMap::EchoMap() :
    worker{[] {
#ifndef __EMSCRIPTEN__
        glfwPostEmptyEvent();
#endif
    }},
#ifdef __EMSCRIPTEN__
    project_controller(
            panel_host,
            worker
    )
#else
    project_controller(
            render_host,
            panel_host,
            worker
    )
#endif
{
    setup_subscriptions();

    panel_host.add_panel(std::make_unique<MenuPanel>(project_controller));
    panel_host.add_panel(std::make_unique<ProjectPanel>());
    panel_host.add_panel(std::make_unique<SignalWaveformPanel>(&worker, despatcher));
    panel_host.add_panel(std::make_unique<SignalDFTPanel>(&worker, despatcher, &render_host));

    // panels.push_back(std::make_unique<SensorGeometryPanel>(this)); // TODO
    // panels.push_back(std::make_unique<ChannelMappingPanel>(this)); // TODO
}

EchoMap::~EchoMap() noexcept = default;

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

void EchoMap::tick()
{
    process_notifications();
    process_worker_results();
}

void EchoMap::setup_subscriptions()
{
    // NOLINTBEGIN(*-redundant-casting) - False positive; casts are required for libsigcpp to resolve overloads.

    connections.emplace_back(despatcher.load_project_finished_channel.nominate_consumer(
            sigc::mem_fun(
                    project_controller,
                    static_cast<void (ProjectController::*)(LoadProjectResult&&)>(&ProjectController::handle_result)
            )
    ));

    connections.emplace_back(despatcher.load_signal_file_channel.nominate_consumer(
            sigc::mem_fun(
                    project_controller,
                    static_cast<void (ProjectController::*)(LoadSignalFileResult&&)>(&ProjectController::handle_result)
            )
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

} // namespace echomap
