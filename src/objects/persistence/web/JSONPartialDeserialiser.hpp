/**
 * @file
 *
 * JSONPartialDeserialiser specification
 *
 * @author Oliver Dixon
 * @date 2026-07-25
 */

#ifndef ECHOMAP_JSONPARTIALDESERIALISER_HPP
#define ECHOMAP_JSONPARTIALDESERIALISER_HPP

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include <filesystem>

#include <simdjson.h>

namespace echomap
{

class PartialProject;
class Worker;

/**
 * JSON parser for PartialProject objects.
 *
 * To parse full Project instances, see JSONFullDeserialiser.
 */
class JSONPartialDeserialiser
{
public:
    std::unique_ptr<PartialProject> deserialise_project(
            const std::filesystem::path& path,
            Worker& worker
    );

private:
    simdjson::ondemand::parser parser;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_JSONPARTIALDESERIALISER_HPP
