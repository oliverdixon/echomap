//
// Created by owd on 07/07/2026.
//

#ifndef ECHOMAP_ISERIALISER_HPP
#define ECHOMAP_ISERIALISER_HPP

#include <string_view>

namespace echomap
{

class Project;
class Sensor;
class Signal;

class ISerialiser
{
public:
    virtual ~ISerialiser() = default;

    virtual std::string_view serialise_project(const Project& project) = 0;
};

} // namespace echomap

#endif // ECHOMAP_ISERIALISER_HPP
