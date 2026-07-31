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
 * Expresses a service for observing the active PartialProject.
 */
class IPartialProjectObserveService
{
public:
    virtual ~IPartialProjectObserveService() = default;

    IPartialProjectObserveService(const IPartialProjectObserveService&) = default;
    IPartialProjectObserveService& operator=(const IPartialProjectObserveService&) = default;
    IPartialProjectObserveService(IPartialProjectObserveService&&) = default;
    IPartialProjectObserveService& operator=(IPartialProjectObserveService&&) = default;

    /**
     * Get an observing pointer to the active PartialProject.
     *
     * @return An immutable view of the active PartialProject, or @c nullptr if no PartialProject is stored.
     */
    [[nodiscard]] virtual const PartialProject* observe_partial_project() const noexcept = 0;

protected:
    IPartialProjectObserveService() = default;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_IPARTIALPROJECTOBSERVESERVICE_HPP
