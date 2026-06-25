//
// Created by owd on 24/06/2026.
//

#ifndef WEBCFD_SENSOR_HPP
#define WEBCFD_SENSOR_HPP

#include "../Object.hpp"

namespace WebCFD
{

class Sensor : public Object<Sensor>
{
public:
    struct Position
    {
        float x;
        float y;
        float z;
    } position{};

    explicit Sensor(std::string_view name = {});
};

} // namespace WebCFD

#endif // WEBCFD_SENSOR_HPP
