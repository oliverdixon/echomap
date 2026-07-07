//
// Created by owd on 07/07/2026.
//

#ifndef WEBCFD_IDESERIALISER_HPP
#define WEBCFD_IDESERIALISER_HPP

#include <memory>
#include <string_view>

namespace WebCFD
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

} // namespace WebCFD

#endif // WEBCFD_IDESERIALISER_HPP
