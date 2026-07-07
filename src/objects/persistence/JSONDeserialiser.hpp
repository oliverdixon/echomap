//
// Created by owd on 07/07/2026.
//

#ifndef ECHOMAP_JSONDESERIALISER_HPP
#define ECHOMAP_JSONDESERIALISER_HPP

#include <simdjson.h>

#include "IDeserialiser.hpp"

namespace EchoMap
{

class JSONDeserialiser : public IDeserialiser
{
public:
    std::unique_ptr<Project> deserialise_project(std::string_view path) override;

private:
    simdjson::ondemand::parser parser;
};

} // namespace EchoMap

#endif // ECHOMAP_JSONDESERIALISER_HPP
