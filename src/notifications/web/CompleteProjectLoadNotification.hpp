/**
 * @file
 *
 * CompleteProjectLoadNotification specification
 *
 * @author Oliver Dixon
 * @date 2026-07-19
 */

#ifndef ECHOMAP_COMPLETEPROJECTLOADNOTIFICATION_HPP
#define ECHOMAP_COMPLETEPROJECTLOADNOTIFICATION_HPP

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "../../objects/IDAllocator.hpp"

namespace echomap
{

class PartialProject;

/**
 * A notification indicating that a pending Project is ready to be loaded.
 *
 * Produced by the IndividualUploadModal once the user has prepared a sufficient set of VFS mappings, such that all
 * externally sourced files are associated with uploads in the WebAssembly VFS.
 *
 * @ingroup Notifications
 */
struct CompleteProjectLoadNotification
{
    /**
     * Create a CompleteProjectLoadNotification to indicate that the referenced Project is ready to be loaded.
     *
     * @param project_id The ID of the Project to be loaded.
     */
    explicit CompleteProjectLoadNotification(id_type project_id);

    /**
     * Verify that the given Project matches the intended target.
     *
     * @param context The context to which the notification will apply.
     * @throws IgnoredWarning The notification does not apply to the given context and should be ignored.
     */
    void verify_project(const PartialProject* context) const;

    id_type project_id; /**< The ID of the Project to be loaded. */
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_COMPLETEPROJECTLOADNOTIFICATION_HPP
