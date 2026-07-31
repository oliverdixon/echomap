/**
 * @file
 *
 * IProjectObserveService specification
 *
 * @author Oliver Dixon
 * @date 2026-07-30
 */

#ifndef ECHOMAP_IPROJECTOBSERVESERVICE_HPP
#define ECHOMAP_IPROJECTOBSERVESERVICE_HPP

namespace echomap
{

class Project;

/**
 * Expresses a service for observing an active Project.
 */
class IProjectObserveService
{
public:
    virtual ~IProjectObserveService() = default;

    IProjectObserveService(const IProjectObserveService&) = default;
    IProjectObserveService& operator=(const IProjectObserveService&) = default;
    IProjectObserveService(IProjectObserveService&&) = default;
    IProjectObserveService& operator=(IProjectObserveService&&) = default;

    /**
     * Get an observing pointer to the active Project.
     *
     * @return An immutable view of the active Project, or @c nullptr if no Project is stored.
     */
    [[nodiscard]] virtual const Project* observe_project() const noexcept = 0;

protected:
    IProjectObserveService() = default;
};

} // namespace echomap

#endif // ECHOMAP_IPROJECTOBSERVESERVICE_HPP
