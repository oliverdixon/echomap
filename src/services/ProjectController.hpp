/**
 * @file
 *
 * ProjectController specification
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#ifndef ECHOMAP_PROJECTCONTROLLER_HPP
#define ECHOMAP_PROJECTCONTROLLER_HPP

#ifdef __EMSCRIPTEN__
#include "web/PartialProjectController.hpp"
#else
#include "native/FullProjectController.hpp"
#endif // __EMSCRIPTEN__

namespace echomap
{

#ifdef __EMSCRIPTEN__
using ProjectController = PartialProjectController;
#else
using ProjectController = FullProjectController;
#endif // __EMSCRIPTEN__

} // namespace echomap

#endif // ECHOMAP_PROJECTCONTROLLER_HPP
