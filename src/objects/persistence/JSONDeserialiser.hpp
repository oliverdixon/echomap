//
// Created by owd on 07/07/2026.
//

#ifndef ECHOMAP_JSONDESERIALISER_HPP
#define ECHOMAP_JSONDESERIALISER_HPP

#include <filesystem>

#include <simdjson.h>

#include "IDeserialiser.hpp"

namespace echomap
{

class JSONDeserialiser : public IDeserialiser
{
public:
    std::unique_ptr<Project> deserialise_project(
            const std::filesystem::path& path,
            Worker* worker
    ) override;

private:
    simdjson::ondemand::parser parser;
};

} // namespace echomap

#endif // ECHOMAP_JSONDESERIALISER_HPP
