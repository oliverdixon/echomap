/**
 * @file
 *
 * IPartialProjectCompletionService specification
 *
 * @author Oliver Dixon
 * @date 2026-07-30
 */

#ifndef ECHOMAP_IPARTIALPROJECTCOMPLETIONSERVICE_HPP
#define ECHOMAP_IPARTIALPROJECTCOMPLETIONSERVICE_HPP

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "../../objects/IDAllocator.hpp"

namespace echomap
{

/**
 * Expresses a service for indicating that a PartialProject is ready to be committed or discarded.
 */
class IPartialProjectCompletionService
{
public:
    virtual ~IPartialProjectCompletionService() = default;

    IPartialProjectCompletionService(const IPartialProjectCompletionService&) = default;
    IPartialProjectCompletionService& operator=(const IPartialProjectCompletionService&) = default;
    IPartialProjectCompletionService(IPartialProjectCompletionService&&) = default;
    IPartialProjectCompletionService& operator=(IPartialProjectCompletionService&&) = default;

    /**
     * Indicate that the specification of a PartialProject has been abandoned.
     *
     * @param intended_project_id The ID of the abandoned PartialProject.
     */
    virtual void cancel_project_load(id_type intended_project_id) = 0;

    /**
     * Indicate that the specification of a PartialProject has been finalised.
     *
     * @param intended_project_id The ID of the populated PartialProject.
     */
    virtual void complete_project_load(id_type intended_project_id) = 0;

protected:
    IPartialProjectCompletionService() = default;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_IPARTIALPROJECTCOMPLETIONSERVICE_HPP
