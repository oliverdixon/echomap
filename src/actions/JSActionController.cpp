/**
 * @file
 *
 * JSActionController implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-18
 */

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "JSActionController.hpp"

#include <emscripten/em_js.h>

#include "../errors/ConfigurationError.hpp"
#include "../utility/Logger.hpp"

namespace echomap
{

void JSActionController::register_vfs_mapping_impl(
        const std::size_t project_id,
        const std::filesystem::path& external
)
{
    // TODO remove.
}

} // namespace echomap


/**
 * Services the @ref RegisterVFSMapping callback for Emscripten.
 *
 * @param project_id The ID of the Project that owns the destination Signal.
 * @param external The path of the external file being mapped into the VFS.
 * @param internal The path of the VFS file.
 *
 * @return Zero status to indicate success; non-zero to indicate failure.
 *
 * @ingroup RegisterVFSMapping
 */
extern "C" EMSCRIPTEN_KEEPALIVE int echomap_on_register_vfs_mapping(
        const std::size_t project_id,
        const char* const external,
        const char* const internal
) noexcept
{
    // TODO remove.
}

#endif // __EMSCRIPTEN__
