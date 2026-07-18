/**
 * @file
 *
 * ActionController specification
 *
 * @author Oliver Dixon
 * @date 2026-07-18
 */

#ifndef ECHOMAP_ACTIONCONTROLLER_HPP
#define ECHOMAP_ACTIONCONTROLLER_HPP

#ifdef __EMSCRIPTEN__
#include "JSActionController.hpp"
#else
#include "NativeActionController.hpp"
#endif

namespace echomap
{

#ifdef __EMSCRIPTEN__
/**
 * Provides an ActionControllerBase implementation alias for Emscripten targets.
 * @ingroup Actions
 */
using ActionController = JSActionController;
#else
/**
 * Provides an ActionControllerBase implementation alias for native (non-Emscripten) targets.
 * @ingroup Actions
 */
using ActionController = NativeActionController;
#endif

} // namespace echomap

#endif // ECHOMAP_ACTIONCONTROLLER_HPP
