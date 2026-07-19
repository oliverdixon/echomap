/**
 * @file
 *
 * StaticInstanceController implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-18
 */

#include "StaticInstanceController.hpp"

#include <sigc++/sigc++.h>

#include "EchoMap.hpp"
#include "actions/ActionController.hpp"

namespace echomap
{

StaticInstanceController::StaticInstanceController(
        EchoMap& app
)
{
    ActionController::bind([&app](const std::filesystem::path& path) {
        app.submit_lightweight_task(ProjectLoadRequest(path));
    },
            [&app](const std::size_t project_id, const std::size_t signal_id, const std::filesystem::path& path) {
                app.submit_lightweight_task(WaveFilePreloadNotification(project_id, signal_id, path));
            });
}

StaticInstanceController::~StaticInstanceController() noexcept
{
    ActionController::unbind();
}

} // namespace echomap
