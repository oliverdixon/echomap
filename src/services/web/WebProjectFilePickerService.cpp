/**
 * @file
 *
 * WebProjectFilePickerService implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "WebProjectFilePickerService.hpp"

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

WebProjectFilePickerService* WebProjectFilePickerService::instance = nullptr;

WebProjectFilePickerService::WebProjectFilePickerService()
{
    if (instance != nullptr)
        throw ConfigurationError("Only one WebProjectFilePicker instance may be active.");

    instance = this;
}

WebProjectFilePickerService::~WebProjectFilePickerService() noexcept
{
    if (instance == this)
        instance = nullptr;
}

void WebProjectFilePickerService::request_project_file(
        SuccessCallbackT success,
        CancelledCallbackT cancelled
)
{
    success_callback = std::move(success);
    cancelled_callback = std::move(cancelled);

    js::select_project_file();
}

WebProjectFilePickerService::Status WebProjectFilePickerService::complete_project_file_pick(
        const char* path
)
{
    if (instance == nullptr)
        return Status::NoBoundInstance;

    if (path == nullptr) {
        if (instance->cancelled_callback)
            instance->cancelled_callback();

        return Status::NoPath;
    }

    const auto callback = std::move(instance->success_callback);
    instance->success_callback = {};
    instance->cancelled_callback = {};

    if (callback)
        callback(std::filesystem::path{path});

    return Status::Success;
}

} // namespace echomap


extern "C" EMSCRIPTEN_KEEPALIVE int echomap_on_project_file_picked(
        const char* const path
) noexcept
{
    using namespace echomap;

    try {
        return std::to_underlying(WebProjectFilePickerService::complete_project_file_pick(path));
    } catch (...) {
        LOG_ERROR("Could not load Project due to unknown error.");
        return -1;
    }
}

#endif // __EMSCRIPTEN__
