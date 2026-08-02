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
#include "../../objects/web/PartialProject.hpp"
#include "../../panels/web/MapSourcesModal.hpp"
#include "../../services/web/VFSPickerService.hpp"
#include "../../services/web/WebProjectFilePickerService.hpp"
#include "../../utility/Logger.hpp"
#include "../PanelHost.hpp"

namespace echomap
{

PartialProjectController::PartialProjectController(
        PanelHost& panel_host,
        Worker& worker
) :
    ProjectControllerBase(
            panel_host,
            std::make_unique<WebProjectFilePickerService>(),
            worker
    ),
    vfs_picker(std::make_unique<VFSPickerService>())
{
}

PartialProjectController::~PartialProjectController() noexcept = default;

const PartialProject* PartialProjectController::observe_partial_project() const noexcept
{
    return partial_project.get();
}

void PartialProjectController::prompt_for_vfs_mapping(
        const id_type intended_project_id,
        const std::filesystem::path& intended_external
)
{
    vfs_picker->request_vfs_mapping(
            intended_project_id,
            intended_external,
            sigc::mem_fun(*this, &PartialProjectController::handle_complete_mapping),
            sigc::ptr_fun(&PartialProjectController::handle_cancelled_mapping)
    );
}

void PartialProjectController::cancel_project_load(
        const id_type intended_project_id
)
{
    if (partial_project == nullptr) {
        LOG_WARN("Ignoring cancellation request due to empty project.");
        return;
    }

    if (partial_project->get_id() != intended_project_id) {
        LOG_F_WARN(
                "Ignoring cancellation request due to invalid project: requested {}, but have {}.",
                intended_project_id,
                partial_project->get_id()
        );
        return;
    }

    panel_host.reset_active_modal();
    partial_project.reset();
}

void PartialProjectController::complete_project_load(
        const id_type intended_project_id
)
{
    if (partial_project == nullptr) {
        LOG_WARN("Ignored completed VFS mapping due to empty project.");
        return;
    }

    if (partial_project->get_id() != intended_project_id) {
        LOG_F_WARN(
                "Ignored completed VFS mapping due to invalid project: requested {}, but have {}.",
                intended_project_id,
                partial_project->get_id()
        );
        return;
    }

    if (!partial_project->all_sources_mapped()) {
        LOG_WARN("Ignored project load request since VFS mappings are not complete.");
        return;
    }

    // For each group, create an asynchronous task to load the corresponding file.

    for (auto&& [vfs_path, factories] : partial_project->drain_unloaded_factories()) {

        if (!vfs_path.has_value()) {
            panel_host.raise_error("Refusing to complete project load due to an incomplete VFS mapping.");
            return;
        }

        // Once these tasks complete, if everything is loaded correctly, we'll change the active project.
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

    if (auto&& new_project = std::move(result).take_project(); !new_project->observe_unloaded_signals().empty()) {
        // Raise the modal to query for the sources.
        panel_host.change_active_modal(std::make_unique<MapSourcesModal>(*this, *this, *this));
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

void PartialProjectController::handle_complete_mapping(
        const id_type project_id,
        const std::filesystem::path& external,
        std::filesystem::path internal
) const
{
    if (partial_project == nullptr) {
        LOG_F_WARN("Ignoring VFS mapping for {} to {} due to empty project.", external.c_str(), internal.c_str());
        return;
    }

    if (partial_project->get_id() != project_id) {
        LOG_F_WARN(
                "Ignoring VFS mapping due to invalid project: requested {}, but have {}.",
                project_id,
                partial_project->get_id()
        );

        return;
    }

    try {
        partial_project->add_vfs_mapping_for_unavailable_signal(external, std::move(internal));
    } catch (const std::runtime_error&) {
        LOG_F_WARN("Ignoring VFS mapping since we don't need a mapping for {}.", external.c_str());
    }
}

void PartialProjectController::handle_cancelled_mapping(
        const std::filesystem::path& external
)
{
    std::ignore = external;
    LOG_F_DEBUG("Cancelled VFS mapping for {}.", external.c_str());
}

} // namespace echomap

#endif // __EMSCRIPTEN__
