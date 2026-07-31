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

VFSPickerService::Status VFSPickerService::complete_vfs_mapping(
        const id_type project_id,
        const char* const external,
        const char* const internal
)
{
    if (instance == nullptr)
        return Status::NoBoundInstance;

    if (external == nullptr)
        return Status::NoPath;

    auto& self = *instance;

    if (!self.pending_request.has_value())
        return Status::NoPendingRequest;

    if (internal == nullptr) {
        if (self.pending_request->cancelled_callback)
            self.pending_request->cancelled_callback(std::filesystem::path{external});
        return Status::NoPath;
    }

    const auto request = std::move(*self.pending_request);
    self.pending_request.reset();

    if (request.project_id != project_id)
        return Status::UnexpectedProject;

    if (request.external != external)
        return Status::UnexpectedPath;

    if (request.success_callback)
        request.success_callback(project_id, request.external, std::filesystem::path{internal});

    return Status::Success;
}

VFSPickerService* VFSPickerService::instance = nullptr;

} // namespace echomap

extern "C" EMSCRIPTEN_KEEPALIVE int echomap_on_register_vfs_mapping(
        const std::size_t project_id,
        const char* const external,
        const char* const internal
) noexcept
{
    using namespace echomap;

    try {
        return std::to_underlying(VFSPickerService::complete_vfs_mapping(project_id, external, internal));
    } catch (...) {
        LOG_ERROR("Could not register mapping due to unknown system error.");
        return -1;
    }
}

#endif // __EMSCRIPTEN__
