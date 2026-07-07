//
// Created by owd on 07/07/2026.
//

#ifndef WEBCFD_JSONDESERIALISER_HPP
#define WEBCFD_JSONDESERIALISER_HPP

#include <simdjson.h>

#include "IDeserialiser.hpp"

namespace WebCFD
{

class JSONDeserialiser : public IDeserialiser
{
public:
    std::unique_ptr<Project> deserialise_project(std::string_view path) override;

private:
    simdjson::ondemand::parser parser;
};

} // namespace WebCFD

#endif // WEBCFD_JSONDESERIALISER_HPP
