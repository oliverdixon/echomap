/**
 * @file
 *
 * RaiseNativeFileChooser specification
 *
 * @author Oliver Dixon
 * @date 2026-07-24
 */

#ifndef ECHOMAP_RAISENATIVEFILECHOOSER_HPP
#define ECHOMAP_RAISENATIVEFILECHOOSER_HPP

#ifndef __EMSCRIPTEN__

namespace echomap
{

/**
 * A notification indicating that the NativeFileChooser should be raised to prompt for a new Project.
 *
 * Produced by the ActionController on ProjectFileAction invocation.
 *
 * @ingroup Notifications ProjectFileAction
 */
struct RaiseNativeFileChooser
{
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_RAISENATIVEFILECHOOSER_HPP
