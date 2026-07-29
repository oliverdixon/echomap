/**
 * @file
 *
 * ProjectFilePicker specification
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#ifndef ECHOMAP_PROJECTFILEPICKER_HPP
#define ECHOMAP_PROJECTFILEPICKER_HPP

#ifdef __EMSCRIPTEN__
// TODO
#else
#include "native/NativeProjectFilePicker.hpp"
#endif // __EMSCRIPTEN__

namespace echomap
{

#ifdef __EMSCRIPTEN__
// TOD
#else
using ProjectFilePicker = NativeProjectFilePicker;
#endif // __EMSCRIPTEN__

} // namespace echomap

#endif // ECHOMAP_PROJECTFILEPICKER_HPP
