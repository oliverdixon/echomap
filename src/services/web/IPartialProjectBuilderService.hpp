/**
 * @file
 *
 * IPartialProjectBuilderService specification
 *
 * @author Oliver Dixon
 * @date 2026-07-30
 */

#ifndef ECHOMAP_IPARTIALPROJECTBUILDERSERVICE_HPP
#define ECHOMAP_IPARTIALPROJECTBUILDERSERVICE_HPP

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "../../objects/IDAllocator.hpp"

namespace echomap
{

/**
 * Expresses a service for incrementally building a PartialProject with Virtual Filesystem (VFS) mappings.
 */
class IPartialProjectBuilderService
{
public:
    virtual ~IPartialProjectBuilderService() = default;

    IPartialProjectBuilderService(const IPartialProjectBuilderService&) = default;
    IPartialProjectBuilderService& operator=(const IPartialProjectBuilderService&) = default;
    IPartialProjectBuilderService(IPartialProjectBuilderService&&) = default;
    IPartialProjectBuilderService& operator=(IPartialProjectBuilderService&&) = default;

    /**
     * Prompts the user to provide a new VFS mapping for a fixed path.
     *
     * @param intended_project_id The ID of the target PartialProject.
     * @param intended_external The path for which a mapping is being sought.
     */
    virtual void prompt_for_vfs_mapping(
            id_type intended_project_id,
            const std::filesystem::path& intended_external
    ) = 0;

protected:
    IPartialProjectBuilderService() = default;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_IPARTIALPROJECTBUILDERSERVICE_HPP
