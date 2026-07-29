/**
 * @file
 *
 * FullProjectController implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#if !defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "FullProjectController.hpp"

#include "../../async/results/LoadProjectResult.hpp"
#include "../../async/results/LoadSignalFileResult.hpp"
#include "../../objects/Project.hpp"
#include "../../objects/Signal.hpp"
#include "../../utility/Logger.hpp"
#include "../PanelHost.hpp"

namespace echomap
{

FullProjectController::FullProjectController(
        PanelHost& panel_host
) :
    ProjectControllerBase(panel_host)
{
}

FullProjectController::~FullProjectController() noexcept = default;

void FullProjectController::handle_result_impl(
        LoadProjectResult&& result
)
{
    if (panel_host.is_modal_shown())
        LOG_WARN("Ignoring request to change active Project since there is an active modal.");
    else
        change_active_project(std::move(result).take_project());
}

void FullProjectController::handle_result_impl(
        LoadSignalFileResult&& result
) const
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

} // namespace echomap


#endif // __EMSCRIPTEN__
