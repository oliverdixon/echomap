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
#include "../../objects/Project.hpp"
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

FullProjectController::~FullProjectController() = default;

void FullProjectController::handle_result(
        LoadProjectResult&& result
)
{
    if (panel_host.is_modal_shown())
        LOG_WARN("Ignoring request to change active Project since there is an active modal.");
    else
        change_active_project(std::move(result).take_project());
}

} // namespace echomap

#endif // __EMSCRIPTEN__
