/**
 * @file
 *
 * CancelProjectLoadNotification implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-22
 */

#include "CancelProjectLoadNotification.hpp"

#include "../errors/IgnoredWarning.hpp"
#include "../objects/Project.hpp"

namespace echomap
{

CancelProjectLoadNotification::CancelProjectLoadNotification(
        const id_type project_id
) :
    project_id(project_id)
{
}

void CancelProjectLoadNotification::verify_project(
        const Project* const context
) const
{
    if (context == nullptr)
        throw IgnoredWarning("Dropping CancelProjectLoadNotification due to empty project.");

    if (context->get_id() != project_id)
        throw IgnoredWarning(
                std::format(
                        "Dropping CancelProjectLoadNotification due to invalid project: requested {}, but have {}.",
                        project_id,
                        context->get_id()
                )
        );
}

} // namespace echomap
