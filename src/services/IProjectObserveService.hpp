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
 * @todo Document
 */
class IProjectObserveService
{
public:
    virtual ~IProjectObserveService() = default;

    [[nodiscard]] virtual const Project* observe_project() const noexcept = 0;
};

} // namespace echomap

#endif // ECHOMAP_IPROJECTOBSERVESERVICE_HPP
