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
    ActionController::bind(sigc::mem_fun(app, &EchoMap::notify));
}

StaticInstanceController::~StaticInstanceController() noexcept
{
    ActionController::unbind();
}

} // namespace echomap
