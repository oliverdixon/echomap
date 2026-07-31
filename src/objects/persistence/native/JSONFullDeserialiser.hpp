/**
 * @file
 *
 * JSONFullDeserialiser specification
 *
 * @author Oliver Dixon
 * @date 2026-07-27
 */

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
 * JSON parser for Project objects.
 *
 * To parse PartialProject instances, see JSONPartialDeserialiser.
 */
class JSONFullDeserialiser
{
public:
    std::unique_ptr<Project> deserialise_project(
            const std::filesystem::path& path,
            Worker& worker
    );

private:
    simdjson::ondemand::parser parser;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_JSONFULLDESERIALISER_HPP
