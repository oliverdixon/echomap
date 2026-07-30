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

/**
 * @todo Document
 */
class IProjectMutationService
{
public:
    virtual ~IProjectMutationService() = default;

    virtual void add_channel_mapping(
            id_type signal_id,
            id_type sensor_id
    ) = 0;

    virtual void modify_sensor_position(
            id_type sensor_id,
            const Position& position
    ) = 0;

    virtual void modify_sensor_colour(
            id_type sensor_id,
            const Colour& colour
    ) = 0;
};

} // namespace echomap

#endif // ECHOMAP_IPROJECTMUTATIONSERVICE_HPP
