/**
 * @file
 *
 * IPartialProjectObserveService specification
 *
 * @author Oliver Dixon
 * @date 2026-07-30
 */

#ifndef ECHOMAP_IPARTIALPROJECTOBSERVESERVICE_HPP
#define ECHOMAP_IPARTIALPROJECTOBSERVESERVICE_HPP

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

namespace echomap
{

class PartialProject;

/**
 * @todo Document
 */
class IPartialProjectObserveService
{
public:
    virtual ~IPartialProjectObserveService() = default;

    [[nodiscard]] virtual const PartialProject* observe_partial_project() const noexcept = 0;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_IPARTIALPROJECTOBSERVESERVICE_HPP
