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

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)
#include "JSActionController.hpp"
#else
#include "NativeActionController.hpp"
#endif

namespace echomap
{

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)
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
