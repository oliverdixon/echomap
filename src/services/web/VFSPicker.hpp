/**
 * @file
 *
 * VFSPicker specification
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#ifndef ECHOMAP_VFSPICKER_HPP
#define ECHOMAP_VFSPICKER_HPP

#include <sigc++/slot.h>

#include <filesystem>

#include "../../objects/IDAllocator.hpp"

extern "C" int echomap_on_register_vfs_mapping(
        std::size_t,
        const char*,
        const char*
) noexcept;

namespace echomap
{

/**
 * @todo Document
 */
class VFSPicker
{
public:
    using SuccessCallbackT =
            sigc::slot<void(id_type project_id, std::filesystem::path external, std::filesystem::path internal)>;
    using CancelledCallbackT = sigc::slot<void(std::filesystem::path external)>;

    VFSPicker();
    ~VFSPicker() noexcept;

    VFSPicker(const VFSPicker&) = delete;
    VFSPicker& operator=(const VFSPicker&) = delete;
    VFSPicker(VFSPicker&&) = delete;
    VFSPicker& operator=(VFSPicker&&) = delete;

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

    static int complete_vfs_mapping(
            id_type project_id,
            const char* external,
            const char* internal
    ) noexcept;

    std::optional<PendingRequest> pending_request;
    static VFSPicker* instance;
};

} // namespace echomap

#endif // ECHOMAP_VFSPICKER_HPP
