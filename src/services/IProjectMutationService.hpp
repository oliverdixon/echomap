/**
 * @file
 *
 * IProjectMutationService specification
 *
 * @author Oliver Dixon
 * @date 2026-07-30
 */

#ifndef ECHOMAP_IPROJECTMUTATIONSERVICE_HPP
#define ECHOMAP_IPROJECTMUTATIONSERVICE_HPP

#include "../objects/IDAllocator.hpp"
#include "../utility/Colour.hpp"
#include "../utility/Position.hpp"

namespace echomap
{

class Signal;
class Sensor;

/**
 * Expresses a service for applying simple changes to an active Project.
 */
class IProjectMutationService
{
public:
    virtual ~IProjectMutationService() = default;

    IProjectMutationService(const IProjectMutationService&) = default;
    IProjectMutationService& operator=(const IProjectMutationService&) = default;
    IProjectMutationService(IProjectMutationService&&) = default;
    IProjectMutationService& operator=(IProjectMutationService&&) = default;

    virtual void add_channel_mapping(
            const Signal& signal,
            const Sensor& sensor
    ) = 0;

    virtual void modify_sensor_position(
            id_type sensor_id,
            const Position& position
    ) = 0;

    virtual void modify_sensor_colour(
            id_type sensor_id,
            const Colour& colour
    ) = 0;

protected:
    IProjectMutationService() = default;
};

} // namespace echomap

#endif // ECHOMAP_IPROJECTMUTATIONSERVICE_HPP
