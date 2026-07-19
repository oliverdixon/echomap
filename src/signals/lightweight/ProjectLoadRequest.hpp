/**
 * @file
 *
 * ProjectLoadRequest specification
 *
 * @author Oliver Dixon
 * @date 2026-07-19
 */

#ifndef ECHOMAP_PROJECTLOADREQUEST_HPP
#define ECHOMAP_PROJECTLOADREQUEST_HPP

#include <filesystem>

namespace echomap
{

/**
 * A lightweight task indicating the a new Project at the specified task should be loaded.
 */
struct ProjectLoadRequest
{
    explicit ProjectLoadRequest(
            std::filesystem::path path
    ) :
        path(std::move(path))
    {
    }

    std::filesystem::path path;
};

} // namespace echomap

#endif // ECHOMAP_PROJECTLOADREQUEST_HPP
