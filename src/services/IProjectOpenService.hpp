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
 * @todo Document
 */
class IProjectOpenService
{
public:
    virtual ~IProjectOpenService() = default;

    virtual void request_open_project() = 0;
};

} // namespace echomap

#endif // ECHOMAP_IPROJECTOPENSERVICE_HPP
