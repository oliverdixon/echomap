/**
 * @file
 *
 * VFSPickerService implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "VFSPickerService.hpp"

#include <emscripten/em_js.h>

#include "../../errors/ConfigurationError.hpp"
#include "../../utility/Logger.hpp"

namespace echomap
{

#ifndef __DOXYGEN__

// (Doxygen can't handle EM_JS declarations.)

namespace js
{

#pragma clang diagnostic push // Ignore unknown attributes for used Emscripten JS linkage.
#pragma clang diagnostic ignored "-Wunknown-attributes"

EM_JS(void,
      register_vfs_mapping,
      (std::size_t project_id,
       const char* external),
      {
          if (Module.echomapOpenVFSFileMapper) {
              const externalPath = UTF8ToString(external);
              Module.echomapOpenVFSFileMapper(project_id, externalPath);
          } else {
              console.error("Module.echomapOpenVFSFileMapper is not installed.");
          }
      });

#pragma clang diagnostic pop

} // namespace js

#endif // __DOXYGEN__

VFSPickerService::VFSPickerService()
{
    if (instance != nullptr)
        throw ConfigurationError("Only one VFSPicker instance may be active.");

    instance = this;
}

VFSPickerService::~VFSPickerService() noexcept
{
    if (instance == this)
        instance = nullptr;
}

void VFSPickerService::request_vfs_mapping(
        id_type project_id,
        const std::filesystem::path& external,
        SuccessCallbackT success_callback,
        CancelledCallbackT cancelled_callback
)
{
    if (pending_request.has_value())
        throw std::runtime_error("A VFS mapping request is already pending.");

    pending_request.emplace(
            project_id,
            external,
            std::move(success_callback),
            std::move(cancelled_callback)
    );

    js::register_vfs_mapping(project_id, pending_request->external.c_str());

}

int VFSPickerService::complete_vfs_mapping(
        const id_type project_id,
        const char* const external,
        const char* const internal
) noexcept
{
    if (instance == nullptr)
        return 1;

    if (external == nullptr || internal == nullptr)
        return 2;

    auto& self = *instance;

    if (!self.pending_request.has_value())
        return 3;

    const auto request = std::move(*self.pending_request);
    self.pending_request.reset();

    if (request.project_id != project_id)
        return 4;

    if (request.external != external)
        return 5;

    if (request.success_callback)
        request.success_callback(project_id, request.external, std::filesystem::path{internal});

    return 0;
}

VFSPickerService* VFSPickerService::instance = nullptr;

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
    using namespace echomap;

    try {
        return VFSPickerService::complete_vfs_mapping(project_id, external, internal);
    } catch (const ConfigurationError& error) {
        LOG_F_ERROR("Could not load path {} due to error: {}", internal, error.what());
        return 6;
    } catch (const std::exception& error) {
        LOG_F_ERROR("Could not load path {} due to unexpected error: {}", internal, error.what());
        return 7;
    } catch (...) {
        LOG_F_ERROR("Could not load path {} due to unknown error.", internal);
        return 8;
    }
}

#endif // __EMSCRIPTEN__
