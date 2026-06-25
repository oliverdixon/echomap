//
// Created by owd on 24/06/2026.
//

#include "Sensor.hpp"

namespace WebCFD
{

template <> constexpr std::string Object<Sensor>::class_name = "Sensor";

Sensor::Sensor(
        const std::string_view name
) :
    Object(name)
{
}

} // namespace WebCFD
