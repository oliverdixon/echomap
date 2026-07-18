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

#include "../Logger.hpp"

namespace echomap
{

/**
 * @defgroup Actions Actions
 * GUI-based actions managed by a specialisation of ActionControllerBase.
 */

/**
 * Provides a target-independent static mechanism for performing "actions", such as raising a file-selector dialog, and
 * invoking callbacks upon action completion.
 *
 * Prior to using any actions with callbacks, callers should call @ref bind to provide callbacks as slots. Slots may be
 * disconnected with @ref unbind.
 *
 * Due to the extensive reliance upon static storage, this class (and any of its inheritors) is not thread-safe. This
 * data model is necessary to accommodate elaborate callback routines, such as Emscripten's @c ccall API, in which
 * calling code cannot reasonably retrieve a pointer to any C++ objects.
 *
 * @ingroup Actions
 */
template <typename Derived> class ActionControllerBase
{
    /**
     * Simple aggregate for callbacks registered to actions and their slot types.
     * @ingroup Actions
     */
    struct Callbacks
    {
        /**
         * @defgroup ProjectFileAction Project File Action
         * Queries the user for the location of a Project file to load.
         * @ingroup Actions
         * @{
         */

        /** Call signature type the @ref ProjectFileAction. */
        using ProjectFile = sigc::slot<void(const std::filesystem::path&)>;
        /** Stored callback for the @ref ProjectFileAction .*/
        ProjectFile project_file_slot;

        /** @} */

        // TODO: more callbacks...
    };

    static Callbacks callbacks;

public:
    /**
     * Bind callbacks as slots.
     *
     * If a callback is empty, callbacks on the corresponding action will be dropped and a warning logged.
     *
     * @param select_project_file_callback_v Callback for 'Select Project File' action.
     *
     * @ingroup Actions
     */
    static void bind(
            Callbacks::ProjectFile&& select_project_file_callback_v
    )
    {
        callbacks.project_file_slot = std::move(select_project_file_callback_v);
    }

    /**
     * Disconnect all callbacks.
     * @ingroup Actions
     */
    static void unbind()
    {
        callbacks.project_file_slot.disconnect();
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

protected:
    /**
     * Executes the callback for the 'Select Project File' action.
     *
     * @param path The path derived from the prompt.
     * @ingroup ProjectFileAction
     */
    static void notify_project_file(
            const std::filesystem::path& path
    )
    {
        if (callbacks.project_file_slot.empty())
            LOG_F_WARN("Dropping project file selection for {} since no application instance is bound.", path.c_str());
        else
            callbacks.project_file_slot(path);
    }
};

template <typename Derived> ActionControllerBase<Derived>::Callbacks ActionControllerBase<Derived>::callbacks{};

} // namespace echomap

#endif // ECHOMAP_ACTIONCONTROLLERBASE_HPP
