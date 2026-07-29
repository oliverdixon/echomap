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
#include "NativeProjectFilePicker.hpp"

namespace echomap
{

FullProjectController::FullProjectController(
        RenderHost& render_host,
        PanelHost& panel_host,
        Worker& worker
) :
    ProjectControllerBase(
            panel_host,
            std::make_unique<NativeProjectFilePicker>(
                    panel_host,
                    render_host
            ),
            worker
    )
{
}

FullProjectController::~FullProjectController() noexcept = default;

void FullProjectController::handle_result(
        LoadProjectResult&& result
)
{
    panel_host.reset_active_modal();
    change_active_project(std::move(result).take_project());
}

void FullProjectController::handle_result(
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

} // namespace echomap

#endif // __EMSCRIPTEN__
