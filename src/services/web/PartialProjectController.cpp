/**
 * @file
 *
 * PartialProjectController implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "PartialProjectController.hpp"

#include "../../async/Worker.hpp"
#include "../../async/results/LoadProjectResult.hpp"
#include "../../async/results/LoadSignalFileResult.hpp"
#include "../../async/tasks/LoadSignalFileTask.hpp"
#include "../../errors/IgnoredWarning.hpp"
#include "../../notifications/web/CompleteProjectLoadNotification.hpp"
#include "../../objects/web/PartialProject.hpp"
#include "../../panels/web/MapSourcesModal.hpp"
#include "../../utility/Logger.hpp"
#include "../PanelHost.hpp"
#include "VFSPicker.hpp"
#include "WebProjectFilePicker.hpp"

namespace echomap
{

PartialProjectController::PartialProjectController(
        PanelHost& panel_host,
        INotificationSink& notification_sink,
        Worker& worker
) :
    ProjectControllerBase(
            panel_host,
            std::make_unique<WebProjectFilePicker>(),
            worker
    ),
    vfs_picker(std::make_unique<VFSPicker>()),
    worker(worker),
    notification_sink(notification_sink)
{
}

PartialProjectController::~PartialProjectController() noexcept = default;

void PartialProjectController::request_vfs_mapping(
        const id_type intended_project_id,
        const std::filesystem::path& intended_external
) const
{
    vfs_picker->request_vfs_mapping(
            intended_project_id,
            intended_external,
            [this](const id_type project_id, const std::filesystem::path& external, std::filesystem::path internal) {
                if (partial_project == nullptr)
                    throw IgnoredWarning("Ignoring VFS mapping due to empty project.");

                if (partial_project->get_id() != project_id)
                    throw IgnoredWarning(
                            std::format(
                                    "Ignoring VFS mapping due to invalid project: requested {}, but have {}.",
                                    project_id,
                                    partial_project->get_id()
                            )
                    );

                try {
                    partial_project->add_vfs_mapping_for_unavailable_signal(external, std::move(internal));
                } catch (const std::runtime_error&) {
                    throw IgnoredWarning(
                            std::format("Ignoring VFS mapping since we don't need a mapping for {}.", external.c_str())
                    );
                }
            },
            [](const std::filesystem::path&) {
            }
    );
}

void PartialProjectController::cancel_project_load(
        const id_type intended_project_id
)
{
    if (partial_project == nullptr)
        throw IgnoredWarning("Ignoring cancellation request due to empty project.");

    if (partial_project->get_id() != intended_project_id)
        throw IgnoredWarning(
                std::format(
                        "Ignoring cancellation request due to invalid project: requested {}, but have {}.",
                        intended_project_id,
                        partial_project->get_id()
                )
        );

    panel_host.reset_active_modal();
    partial_project.reset();
}

void PartialProjectController::handle_notification(
        const CompleteProjectLoadNotification& notification
) const
{
    notification.verify_project(partial_project.get());

    // For each group, create a worker notification to load the corresponding file.

    for (auto&& [vfs_path, factories] : partial_project->take_unloaded_factories()) {

        if (!vfs_path.has_value())
            throw std::runtime_error("Refusing CompleteProjectLoadNotification due to an incomplete VFS mapping.");

        // Once these notifications return, if everything is loaded correctly, we'll change the active project.
        worker.submit(std::make_unique<LoadSignalFileTask>(partial_project->get_id(), *vfs_path, std::move(factories)));
    }

    panel_host.reset_active_modal();
}

void PartialProjectController::handle_result(
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
        panel_host.change_active_modal(std::make_unique<MapSourcesModal>(*this, notification_sink, new_project.get()));
        partial_project = std::move(new_project);
    } else
        change_active_project(std::move(new_project));
}

void PartialProjectController::handle_result(
        LoadSignalFileResult&& result
)
{
    Project* target = nullptr;

    // Determine whether the result relates to a Signal bound to the active Project or the unloaded Project.

    if (project != nullptr && result.get_project_id() == project->get_id())
        target = project.get();
    else if (partial_project != nullptr && result.get_project_id() == partial_project->get_id())
        target = partial_project.get();

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

    if (target == partial_project.get() && partial_project->observe_unloaded_signals().empty())
        change_active_project(std::move(partial_project));
}

} // namespace echomap

#endif // __EMSCRIPTEN__
