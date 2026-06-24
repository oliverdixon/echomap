//
// Created by owd on 24/06/2026.
//

#ifndef WEBCFD_SENSOR_HPP
#define WEBCFD_SENSOR_HPP

namespace WebCFD
{

class Sensor
{
public:
    struct Position
    {
        float x;
        float y;
        float z;
    } position;
};

} // namespace WebCFD

#endif // WEBCFD_SENSOR_HPP
