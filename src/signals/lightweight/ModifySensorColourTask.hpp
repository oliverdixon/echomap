/**
 * @file
 *
 * ModifySensorColourTask specification
 *
 * @author Oliver Dixon
 * @date 2026-07-14
 */

#ifndef ECHOMAP_MODIFYSENSORCOLOURTASK_HPP
#define ECHOMAP_MODIFYSENSORCOLOURTASK_HPP

#include "../../objects/Sensor.hpp"

namespace echomap
{

/**
 * A lightweight task indicating that the colour of Sensor identified by the given ID, in the active Project, should be
 * changed to the detained colour.
 */
struct ModifySensorColourTask
{
    explicit ModifySensorColourTask(
            const Sensor::id_type sensor_id,
            const Sensor::Colour& colour
    ) :
        sensor_id(sensor_id),
        colour(colour)
    {
    }

    Sensor::id_type sensor_id;
    Sensor::Colour colour;
};

} // namespace echomap

#endif // ECHOMAP_MODIFYSENSORCOLOURTASK_HPP
