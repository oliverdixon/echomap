//
// Created by owd on 07/07/2026.
//

#ifndef ECHOMAP_JSONFULLDESERIALISER_HPP
#define ECHOMAP_JSONFULLDESERIALISER_HPP

#if !defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include <filesystem>

#include <simdjson.h>

namespace echomap
{

class Project;
class Worker;

/**
 * @todo Document.
 */
class JSONFullDeserialiser
{
public:
    std::unique_ptr<Project> deserialise_project(
            const std::filesystem::path& path,
            Worker* worker
    );

private:
    simdjson::ondemand::parser parser;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_JSONFULLDESERIALISER_HPP
