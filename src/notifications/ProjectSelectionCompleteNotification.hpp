/**
 * @file
 *
 * ProjectSelectionCompleteNotification specification
 *
 * @author Oliver Dixon
 * @date 2026-07-19
 */

#ifndef ECHOMAP_PROJECTSELECTIONCOMPLETENOTIFICATION_HPP
#define ECHOMAP_PROJECTSELECTIONCOMPLETENOTIFICATION_HPP

#include <filesystem>

namespace echomap
{

/**
 * A notification indicating that a new Project file has been selected by the user, or the load operation was cancelled.
 *
 * Produced by the ActionController on ProjectFileAction callbacks to indicate a new Project file has been selected for
 * loading into the instance. On native platforms, the FileChooser modal may also produce a
 * ProjectSelectionCompleteNotification.
 *
 * @ingroup Notifications ProjectFileAction
 */
struct ProjectSelectionCompleteNotification
{
    /**
     * Create a new ProjectSelectionCompleteNotification indicating that the load was cancelled.
     */
    ProjectSelectionCompleteNotification() = default;

    /**
     * Create a new ProjectSelectionCompleteNotification.
     *
     * @param path The path of the Project file to load.
     */
    explicit ProjectSelectionCompleteNotification(
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

#endif // ECHOMAP_PROJECTSELECTIONCOMPLETENOTIFICATION_HPP
