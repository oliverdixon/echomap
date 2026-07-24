/**
 * @file
 *
 * ProjectSelectedNotification specification
 *
 * @author Oliver Dixon
 * @date 2026-07-19
 */

#ifndef ECHOMAP_PROJECTSELECTEDNOTIFICATION_HPP
#define ECHOMAP_PROJECTSELECTEDNOTIFICATION_HPP

#include <filesystem>

namespace echomap
{

/**
 * A notification indicating that a new Project file has been selected by the user to load into the application.
 *
 * Produced by the ActionController on ProjectFileAction callbacks to indicate a new Project file has been selected for
 * loading into the instance. On native platforms, the FileChooser modal may also produce a ProjectSelectedNotification.
 *
 * @ingroup Notifications ProjectFileAction
 */
struct ProjectSelectedNotification
{
    /**
     * Create a new ProjectSelectedNotification indicating that the load was cancelled.
     */
    ProjectSelectedNotification() = default;

    /**
     * Create a new ProjectSelectedNotification.
     *
     * @param path The path of the Project file to load.
     */
    explicit ProjectSelectedNotification(
            std::filesystem::path path
    ) :
        path(std::move(path))
    {
    }

    /**
     * The path of the Project file to load.
     *
     * If empty, the operation was cancelled.
     */
    std::optional<std::filesystem::path> path;
};

} // namespace echomap

#endif // ECHOMAP_PROJECTSELECTEDNOTIFICATION_HPP
