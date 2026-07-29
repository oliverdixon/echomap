/**
 * @file
 *
 * EchoMapWeb implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-24
 */

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "EchoMapWeb.hpp"

#include <emscripten/emscripten.h>

#include "../async/tasks/LoadSignalFileTask.hpp"
#include "../errors/IgnoredWarning.hpp"
#include "../notifications/AllNotifications.hpp"
#include "../objects/Signal.hpp"
#include "../objects/web/PartialProject.hpp"
#include "../panels/web/MapSourcesModal.hpp"
#include "../utility/Logger.hpp"

#ifndef __EMSCRIPTEN_PTHREADS__
#warning "The Emscripten application will be single-threaded."
#endif // __EMSCRIPTEN_PTHREADS__

namespace echomap
{

void EchoMapWeb::visit_notification(
        Notification notification
)
{
    std::visit(
            // clang-format off

            variant_helpers::Overloaded{
                make_common_notification_visitors(),
                [this](const CancelProjectLoadNotification& n) { handle_notification(n); },
                [this](const CompleteProjectLoadNotification& n) { handle_notification(n); },
                [this](RegisterVFSMappingNotification& n) { handle_notification(n); },
            },

            // clang-format on
            notification
    );
}

void EchoMapWeb::handle_result(
        LoadProjectResult&& result
)
{
    if (panel_host.is_modal_shown()) {
        LOG_WARN("Ignoring request to change active Project since there is an active modal.");
        return;
    }

    auto&& new_project = std::move(result).take_project();

    if (!new_project->observe_unloaded_signals().empty()) {
        // Raise the modal to query for the sources.
#if 0 // TODO
        active_modal = std::make_unique<MapSourcesModal>(this, new_project.get());
#endif
        unloaded_project = std::move(new_project);
    } else
        change_active_project(std::move(new_project));
}

void EchoMapWeb::handle_result(
        LoadSignalFileResult&& result
)
{
    Project* target = nullptr;

    // Determine whether the result relates to a Signal bound to the active Project or the unloaded Project.

    if (project != nullptr && result.get_project_id() == project->get_id())
        target = project.get();
    else if (unloaded_project != nullptr && result.get_project_id() == unloaded_project->get_id())
        target = unloaded_project.get();

    if (target == nullptr) {
        LOG_F_WARN(
                "Dropping LoadSignalFileResult, which was intended for the unavailable Project with ID {}.",
                result.get_project_id()
        );

        return;
    }

    // Add the signals to the target Project.

    for (auto&& signals = std::move(result).take_signals(); auto signal : signals | std::views::as_rvalue)
        target->add_signal(std::move(signal));

    // If it was an unloaded Project, and all signals are now loaded, it can become the active Project.

    if (target == unloaded_project.get() && unloaded_project->observe_unloaded_signals().empty())
        change_active_project(std::move(unloaded_project));
}

void EchoMapWeb::handle_notification(
        const CancelProjectLoadNotification& notification
)
{
    notification.verify_project(unloaded_project.get());

    panel_host.reset_active_modal();
    unloaded_project.reset();
}

void EchoMapWeb::handle_notification(
        const CompleteProjectLoadNotification& notification
)
{
    notification.verify_project(unloaded_project.get());

    // For each group, create a worker notification to load the corresponding file.

    for (auto&& [vfs_path, factories] : unloaded_project->take_unloaded_factories()) {

        if (!vfs_path.has_value())
            throw std::runtime_error("Refusing CompleteProjectLoadNotification due to an incomplete VFS mapping.");

        // Once these notifications return, if everything is loaded correctly, we'll change the active project.
        worker.submit(
                std::make_unique<LoadSignalFileTask>(unloaded_project->get_id(), *vfs_path, std::move(factories))
        );
    }

    panel_host.reset_active_modal();
}

void EchoMapWeb::handle_notification(
        RegisterVFSMappingNotification& notification
) const
{
    notification.verify_project(unloaded_project.get());

    try {
        unloaded_project->add_vfs_mapping_for_unavailable_signal(
                notification.external,
                std::move(notification.internal)
        );
    } catch (const std::runtime_error&) {
        throw IgnoredWarning(
                std::format(
                        "Dropping RegisterVFSMappingNotification since we don't need a mapping for {}.",
                        notification.external.c_str()
                )
        );
    }
}

void EchoMapWeb::render_shim(
        void* const echomap_instance
)
{
    auto* instance = static_cast<EchoMapWeb*>(echomap_instance);

    instance->tick();
    instance->render_host.render(instance->panel_host);
}

void EchoMapWeb::run_event_loop()
{
    emscripten_set_main_loop_arg(&EchoMapWeb::render_shim, this, 0, true);
}

EchoMapWeb::EchoMapWeb() = default;

EchoMapWeb::~EchoMapWeb() = default;

} // namespace echomap

#endif // __EMSCRIPTEN__
