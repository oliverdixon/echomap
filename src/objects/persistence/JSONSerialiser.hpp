//
// Created by owd on 07/07/2026.
//

#ifndef WEBCFD_JSONSERIALISER_HPP
#define WEBCFD_JSONSERIALISER_HPP

#include <simdjson.h>

#include "ISerialiser.hpp"

namespace WebCFD
{

class JSONSerialiser : public ISerialiser
{
public:
    std::string_view serialise_project(const Project& project) override;

    static std::string pretty_print(std::string_view data);

private:
    simdjson::builder::string_builder sb;
};

} // namespace WebCFD

#endif // WEBCFD_JSONSERIALISER_HPP
