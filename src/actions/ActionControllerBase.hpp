/**
 * @file
 *
 * ActionControllerBase specification
 *
 * @author Oliver Dixon
 * @date 2026-07-18
 */

#ifndef ECHOMAP_ACTIONCONTROLLERBASE_HPP
#define ECHOMAP_ACTIONCONTROLLERBASE_HPP

#include <sigc++/slot.h>

#include <filesystem>

#include "../notifications/AllNotifications.hpp"
#include "../utility/Logger.hpp"
#include "../utility/VariantHelpers.hpp"

namespace echomap
{

/**
 * @defgroup Actions Actions
 * GUI-based actions managed by a specialisation of ActionControllerBase.
 */

/**
 * @defgroup ProjectFileAction Project File Action
 * Queries the user for the location of a Project file to load.
 * @ingroup Actions
 */

/**
 * @defgroup RegisterVFSMapping Register VFS Mapping
 * Queries the user for the location of an externally sourced file.
 * @ingroup Actions
 */

/**
 * Provides a platform-independent static mechanism for performing "system actions", such as raising a file-selector
 * dialog, and invoking callbacks upon completion.
 *
 * The intended workflow is:
 *  -# Application controller invokes ActionControllerBase::bind to provide a slot capable of accepting Notification
 *     objects.
 *  -# Components <b>on the main thread</b> invoke actions, such as ActionControllerBase::select_project_file.
 *  -# The specialisation of ActionControllerBase (either JSActionController or NativeActionController) carries out the
 *     action. This may or may not involve invoking the bound callback slot with a constructed Notification.
 *  -# Following completion of the action, the specialisation reports its status through the bound callback slot with
 *     a notification.
 *
 * Due to the reliance upon static storage, this class (and any of its inheritors) is not thread-safe. This data model
 * is necessary to accommodate elaborate callback routines, such as Emscripten's @c ccall API, in which calling code
 * cannot reasonably retrieve a pointer to any C++ objects.
 *
 * @ingroup Actions
 */
template <typename Derived> class ActionControllerBase
{
    using CallbackT = sigc::slot<void(const Notification&)>;
    static CallbackT callback;

public:
    /**
     * Bind the callback slot.
     *
     * @ingroup Actions
     */
    static void bind(
            CallbackT&& bound_callback
    )
    {
        callback = std::move(bound_callback);
    }

    /**
     * Disconnect the callback.
     *
     * @ingroup Actions
     */
    static void unbind()
    {
        callback.disconnect();
    }

    /**
     * Invokes the @ref ProjectFileAction.
     *
     * @ingroup ProjectFileAction
     */
    static void select_project_file()
    {
        Derived::select_project_file_impl();
    }

    /**
     * Invokes the @ref RegisterVFSMapping.
     *
     * @param project_id The ID of the Project that owns the destination Signal.
     * @param external The path of the external file being mapped into the VFS.
     *
     * @ingroup RegisterVFSMapping
     */
    static void register_vfs_mapping(
            const std::size_t project_id,
            const std::filesystem::path& external
    )
    {
        Derived::register_vfs_mapping_impl(project_id, external);
    }

protected:
    /**
     * Invokes the notification callback.
     *
     * If no callback is bound, a warning is issued and the Notification is dropped.
     *
     * @tparam NotificationT The type of Notification to send.
     * @tparam Args The parameter types of the Notification constructor.
     * @param args The arguments for the Notification constructor.
     */
    template <
            class NotificationT,
            class... Args>
        requires variant_helpers::VariantAlternative<
                NotificationT,
                Notification>
    static void notify(
            Args&&... args
    )
    {
        if (callback.empty())
            LOG_F_WARN(
                    "Dropping {} due to unbound application instance.", NotificationNames::get<NotificationT>()
            );
        else
            callback(NotificationT(std::forward<Args>(args)...));
    }
};

template <typename Derived> ActionControllerBase<Derived>::CallbackT ActionControllerBase<Derived>::callback{};

} // namespace echomap

#endif // ECHOMAP_ACTIONCONTROLLERBASE_HPP
