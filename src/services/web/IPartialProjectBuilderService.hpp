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

namespace echomap
{

/**
 * @todo Document
 */
class IPartialProjectBuilderService
{
public:
    virtual ~IPartialProjectBuilderService() = default;

    virtual void add_vfs_mapping(
            id_type intended_project_id,
            const std::filesystem::path& intended_external
    ) = 0;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_IPARTIALPROJECTBUILDERSERVICE_HPP
