/**
 * @file
 *
 * JSActionController implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-18
 */

#include "JSActionController.hpp"

#include <emscripten/em_js.h>

#include "../Logger.hpp"
#include "../errors/ConfigurationError.hpp"

namespace echomap
{

#ifndef DOXYGEN_SKIP

// (Doxygen can't handle EM_JS declarations.)

/**
 * JavaScript functions to service actions.
 *
 * The JS function signatures are defined as @c extern C++ in the @ref echomap::js namespace. They are called using the
 * standard C++ calling convention from JSActionController member functions, and typically invoke an exported C-linkage
 * free function as a callback following completion of the action.
 */
namespace js
{

#pragma clang diagnostic push // Ignore unknown attributes for used Emscripten JS linkage.
#pragma clang diagnostic ignored "-Wunknown-attributes"

EM_JS(void,
      select_project_file,
      (),
      {
          if (Module.echomapOpenProjectFileChooser) {
              Module.echomapOpenProjectFileChooser();
          } else {
              console.error("Module.echomapOpenProjectFileChooser is not installed.");
          }
      });

#pragma clang diagnostic pop

} // namespace js

#endif

void JSActionController::select_project_file_impl()
{
    js::select_project_file();
}

void JSActionController::notify_project_file_impl(
        const std::filesystem::path& path
)
{
    notify_project_file(path);
}

} // namespace echomap

/**
 * Services the 'Select Project File' callback for Emscripten.
 *
 * @param path The file-system path (in the Wasm VFS) selected in the prompt.
 * @return Zero status to indicate success; non-zero to indicate failure.
 *
 * @ingroup ProjectFileAction
 */
extern "C" EMSCRIPTEN_KEEPALIVE int echomap_on_project_file_picked(
        const char* const path
) noexcept
{
    using namespace echomap;

    if (path == nullptr)
        return 2;

    try {
        JSActionController::notify_project_file_impl(path);
        return 0;
    } catch (const ConfigurationError& error) {
        LOG_F_ERROR("Could not load path {} due to error: {}", path, error.what());
        return 3;
    } catch (const std::exception& error) {
        LOG_F_ERROR("Could not load path {} due to unexpected error: {}", path, error.what());
        return 4;
    } catch (...) {
        LOG_F_ERROR("Could not load path {} due to unknown error.", path);
        return 5;
    }
}
