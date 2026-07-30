/**
 * @file
 *
 * IVFSRequestService specification
 *
 * @author Oliver Dixon
 * @date 2026-07-30
 */

#ifndef ECHOMAP_IVFSREQUESTSERVICE_HPP
#define ECHOMAP_IVFSREQUESTSERVICE_HPP

#include <filesystem>

#include "../../objects/IDAllocator.hpp"

namespace echomap
{

/**
 * @todo Document
 */
class IVFSRequestService
{
public:
    virtual ~IVFSRequestService() = default;

    virtual void request_vfs_mapping(
            id_type intended_project_id,
            const std::filesystem::path& intended_external
    ) = 0;
};

} // namespace echomap

#endif // ECHOMAP_IVFSREQUESTSERVICE_HPP
