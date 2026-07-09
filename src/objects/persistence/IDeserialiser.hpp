//
// Created by owd on 07/07/2026.
//

#ifndef ECHOMAP_IDESERIALISER_HPP
#define ECHOMAP_IDESERIALISER_HPP

#include <memory>
#include <string_view>

namespace echomap
{

class Project;
class Sensor;
class Signal;

class IDeserialiser
{
public:
    virtual ~IDeserialiser() = default;

    virtual std::unique_ptr<Project> deserialise_project(std::string_view path) = 0;
};

} // namespace echomap

#endif // ECHOMAP_IDESERIALISER_HPP
