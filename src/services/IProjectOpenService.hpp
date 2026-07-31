/**
 * @file
 *
 * IProjectOpenService specification
 *
 * @author Oliver Dixon
 * @date 2026-07-30
 */

#ifndef ECHOMAP_IPROJECTOPENSERVICE_HPP
#define ECHOMAP_IPROJECTOPENSERVICE_HPP

namespace echomap
{

/**
 * Expresses a service for initiating a workflow to load a new Project.
 */
class IProjectOpenService
{
public:
    virtual ~IProjectOpenService() = default;

    IProjectOpenService(const IProjectOpenService&) = default;
    IProjectOpenService& operator=(const IProjectOpenService&) = default;
    IProjectOpenService(IProjectOpenService&&) = default;
    IProjectOpenService& operator=(IProjectOpenService&&) = default;

    virtual void request_open_project() = 0;

protected:
    IProjectOpenService() = default;
};

} // namespace echomap

#endif // ECHOMAP_IPROJECTOPENSERVICE_HPP
