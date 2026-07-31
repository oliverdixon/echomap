/**
 * @file
 *
 * VFSPickerService specification
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#ifndef ECHOMAP_VFSPICKERSERVICE_HPP
#define ECHOMAP_VFSPICKERSERVICE_HPP

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include <sigc++/slot.h>

#include <filesystem>

#include "../../objects/IDAllocator.hpp"

/**
 * Services the @ref RegisterVFSMapping callback for Emscripten.
 *
 * @param project_id The ID of the PartialProject.
 * @param external The path of the external file being mapped into the VFS.
 * @param internal The path of the VFS file.
 *
 * @return Status code; see @ref echomap::VFSPickerService::Status, or -1 if an implementation-defined exception was
 *  caught.
 */
extern "C" int echomap_on_register_vfs_mapping(
        std::size_t project_id,
        const char* external,
        const char* internal
) noexcept;

namespace echomap
{

/**
 * Provides a service implementation for selecting a VFS mapping.
 *
 * This class uses static storage to persist object handles across FFI boundaries. Hence, the VFSPickerService is not
 * thread-safe, and cannot be copied or moved.
 */
class VFSPickerService
{
public:
    using SuccessCallbackT =
            sigc::slot<void(id_type project_id, std::filesystem::path external, std::filesystem::path internal)>;
    using CancelledCallbackT = sigc::slot<void(std::filesystem::path external)>;

    VFSPickerService();
    ~VFSPickerService() noexcept;

    VFSPickerService(const VFSPickerService&) = delete;
    VFSPickerService& operator=(const VFSPickerService&) = delete;
    VFSPickerService(VFSPickerService&&) = delete;
    VFSPickerService& operator=(VFSPickerService&&) = delete;

    /**
     * Prompt for a VFS mapping for the given external path.
     *
     * @param project_id The ID of the relevant PartialProject.
     * @param external The external path to be mapped.
     * @param success_callback Callback for the user selecting a file.
     * @param cancelled_callback Callback for the user cancelling the operation.
     */
    void request_vfs_mapping(
            id_type project_id,
            const std::filesystem::path& external,
            SuccessCallbackT success_callback,
            CancelledCallbackT cancelled_callback
    );

private:
    friend int ::echomap_on_register_vfs_mapping(
            std::size_t,
            const char*,
            const char*
    ) noexcept;

    struct PendingRequest
    {
        id_type project_id;
        std::filesystem::path external;
        SuccessCallbackT success_callback;
        CancelledCallbackT cancelled_callback;
    };

    /**
     * Status codes to indicate result to JS runtime.
     */
    enum class Status : std::uint8_t
    {
        Success = 0,       /**< No error reported. */
        NoBoundInstance,   /**< No VFSPickerService instance was bound into static storage. */
        NoPath,            /**< The external or internal path was not provided by the JS callback. */
        NoPendingRequest,  /**< There is no PendingRequest in @ref pending_request. */
        UnexpectedProject, /**< The JS callback indicated a PartialProject with a different ID to ours. */
        UnexpectedPath,    /**< The JS callback indicated a path different to the one we expected. */
    };

    /**
     * Completes the VFS mapping prompt workflow from a static context.
     *
     * If an internal path was not provided, a cancellation is assumed and @ref PendingRequest::cancelled_callback is
     * invoked.
     *
     * @param project_id The ID of the PartialProject.
     * @param external The path of the external file being mapped into the VFS.
     * @param internal The path of the VFS file.
     *
     * @return Integral Status code.
     */
    static Status complete_vfs_mapping(
            id_type project_id,
            const char* external,
            const char* internal
    );

    std::optional<PendingRequest> pending_request;
    static VFSPickerService* instance;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_VFSPICKERSERVICE_HPP
