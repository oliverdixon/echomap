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
 * @todo Document
 */
class IPartialProjectCompletionService
{
public:
    virtual ~IPartialProjectCompletionService() = default;

    virtual void cancel_project_load(id_type intended_project_id) = 0;

    virtual void complete_project_load(id_type intended_project_id) = 0;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_IPARTIALPROJECTCOMPLETIONSERVICE_HPP
