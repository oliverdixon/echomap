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
    static VFSPickerService* instance;
};

} // namespace echomap

#endif // ECHOMAP_VFSPICKERSERVICE_HPP
