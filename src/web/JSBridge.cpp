/**
 * @file
 * @brief JavaScript-C++ bridge implementation
 * @author Oliver Dixon
 * @date 2026-06-24
 */

#ifdef __EMSCRIPTEN__

#include "JSBridge.hpp"

#include <emscripten/em_js.h>
#include <emscripten/em_macros.h>

#include "../Logger.hpp"
#include "../errors/ConfigurationError.hpp"

namespace echomap::web
{

EchoMap* JSBridge::instance = nullptr;

/**
 * EM_JS free functions in the @ref echomap::web namespace to generate JavaScript targets.
 */
namespace js
{

EM_JS(void,
      open_wav_file_chooser_for_existing_signal,
      (int project_id,
       int signal_id),
      {
          if (Module.echomapOpenWAVFileChooserForExistingSignal) {
              Module.echomapOpenWAVFileChooserForExistingSignal(project_id, signal_id);
          } else {
              console.error("Module.echomapOpenWAVFileChooserForExistingSignal is not installed.");
          }
      });

EM_JS(void,
      open_project_file_chooser,
      (),
      {
          if (Module.echomapOpenProjectFileChooser) {
              Module.echomapOpenProjectFileChooser();
          } else {
              console.error("Module.echomapOpenProjectFileChooser is not installed.");
          }
      });

} // namespace js

void JSBridge::bind(
        EchoMap* const new_instance
) noexcept
{
    instance = new_instance;
}

void JSBridge::unbind() noexcept
{
    instance = nullptr;
}

int JSBridge::notify_wav_file_picked_for_existing_signal(
        const std::size_t project_id,
        const std::size_t signal_id,
        const char* const path
) noexcept
{
    if (!preamble())
        return 1;

    if (path == nullptr) {
        LOG_ERROR("File picker callback received no path.");
        return 2;
    }

    try {
        instance->update_wav_file_for_existing_signal(project_id, signal_id, path);
        return 0;
    } catch (const ConfigurationError& error) {
        LOG_F_ERROR("Could not load path {} due to error: {}", path, error.what());
        return 3;
    }
}

void JSBridge::open_wav_file_chooser_for_existing_signal(
        const std::size_t project_id,
        const std::size_t signal_id
) noexcept
{
    js::open_wav_file_chooser_for_existing_signal(static_cast<int>(project_id), static_cast<int>(signal_id));
}

int JSBridge::notify_project_file_picked(
        const char* const path
) noexcept
{
    if (!preamble())
        return 1;

    if (path == nullptr) {
        LOG_ERROR("File picker callback received no path.");
        return 2;
    }

    try {
        instance->update_project(path);
        return 0;
    } catch (const ConfigurationError& error) {
        LOG_F_ERROR("Could not load path {} due to error: {}", path, error.what());
        return 3;
    }
}

void JSBridge::open_project_file_chooser() noexcept
{
    js::open_project_file_chooser();
}

bool JSBridge::preamble() noexcept
{
    if (instance == nullptr) {
        LOG_ERROR("No EchoMap app instance is registered");
        return false;
    }

    return true;
}

} // namespace echomap::web

/**
 * Action to handle a new wave file being selected on the DOM to link to an existing Project and Signal.
 *
 * @param project_id Numerical ID of the Project to which the Signal data belongs.
 * @param signal_id Numerical ID of the Signal to which the data belongs.
 * @param path Path of the selected file in the Wasm file system.
 *
 * @return Numerical status code; non-zero on failure.
 *
 * @see JSBridge::notify_wav_file_picked
 */
extern "C" EMSCRIPTEN_KEEPALIVE int echomap_on_wav_file_picked_for_existing_signal(
        const int project_id,
        const int signal_id,
        const char* const path
)
{
    return echomap::web::JSBridge::notify_wav_file_picked_for_existing_signal(project_id, signal_id, path);
}

extern "C" EMSCRIPTEN_KEEPALIVE int echomap_on_project_file_picked(
        const char* const path
)
{
    return echomap::web::JSBridge::notify_project_file_picked(path);
}

#endif // __EMSCRIPTEN__
