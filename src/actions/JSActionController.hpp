/**
 * @file
 *
 * JSActionController specification
 *
 * @author Oliver Dixon
 * @date 2026-07-18
 */

#ifndef ECHOMAP_JSACTIONCONTROLLER_HPP
#define ECHOMAP_JSACTIONCONTROLLER_HPP

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "ActionControllerBase.hpp"

namespace echomap
{

/**
 * Specialisation of ActionControllerBase to service actions on a WebAssembly target (using the Emscripten runtime).
 *
 * @ingroup Actions
 */
class JSActionController : public ActionControllerBase<JSActionController>
{
    friend ActionControllerBase;

    /**
     * Invokes the JS function for @ref RegisterVFSMapping.
     *
     * @param project_id The ID of the Project that owns the destination Signal.
     * @param external The path of the external file being mapped into the VFS.
     *
     * @ingroup RegisterVFSMapping
     * @implements ActionControllerBase::register_vfs_mapping
     */
    static void register_vfs_mapping_impl(
            std::size_t project_id,
            const std::filesystem::path& external
    );
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_JSACTIONCONTROLLER_HPP
