/**
 * @file
 *
 * CompleteProjectLoadNotification implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-20
 */

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "CompleteProjectLoadNotification.hpp"

#include "../../errors/IgnoredWarning.hpp"
#include "../../objects/web/PartialProject.hpp"

namespace echomap
{

CompleteProjectLoadNotification::CompleteProjectLoadNotification(
        const id_type project_id
) :
    project_id(project_id)
{
}

void CompleteProjectLoadNotification::verify_project(
        const PartialProject* const context
) const
{
    if (context == nullptr)
        throw IgnoredWarning("Dropping CompleteProjectLoadNotification due to empty project.");

    if (context->get_id() != project_id)
        throw IgnoredWarning(
                std::format(
                        "Dropping CompleteProjectLoadNotification due to invalid project: requested {}, but have {}.",
                        project_id,
                        context->get_id()
                )
        );
}

} // namespace echomap

#endif // __EMSCRIPTEN__
