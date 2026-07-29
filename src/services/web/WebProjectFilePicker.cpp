/**
 * @file
 *
 * WebProjectFilePicker implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "WebProjectFilePicker.hpp"

#include <emscripten/em_js.h>

#include "../../errors/ConfigurationError.hpp"
#include "../../utility/Logger.hpp"

namespace echomap
{

#ifndef __DOXYGEN__

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

#endif // __DOXYGEN__

WebProjectFilePicker* WebProjectFilePicker::instance = nullptr;

WebProjectFilePicker::WebProjectFilePicker()
{
    if (instance != nullptr)
        throw ConfigurationError("Only one WebProjectFilePicker instance may be active.");

    instance = this;
}

WebProjectFilePicker::~WebProjectFilePicker() noexcept
{
    if (instance == this)
        instance = nullptr;
}

void WebProjectFilePicker::request_project_file(
        SuccessCallbackT success,
        CancelledCallbackT cancelled
)
{
    success_callback = std::move(success);
    cancelled_callback = std::move(cancelled);

    js::select_project_file();
}

int WebProjectFilePicker::complete_project_file_pick(
        const char* path
) noexcept
{
    if (instance == nullptr)
        return 1;

    if (path == nullptr) {
        if (instance->cancelled_callback)
            instance->cancelled_callback();

        return 2;
    }

    const std::filesystem::path selected_path{path};

    const auto callback = std::move(instance->success_callback);
    instance->success_callback = {};
    instance->cancelled_callback = {};

    if (callback)
        callback(selected_path);

    return 0;
}

} // namespace echomap

/**
 * Services the @ref ProjectFileAction callback for Emscripten.
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

    try {
        return WebProjectFilePicker::complete_project_file_pick(path);
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

#endif // __EMSCRIPTEN__
