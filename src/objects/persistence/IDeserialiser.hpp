//
// Created by owd on 07/07/2026.
//

#ifndef ECHOMAP_IDESERIALISER_HPP
#define ECHOMAP_IDESERIALISER_HPP

#include <memory>
#include <string_view>

namespace echomap
{
class Worker;

class Project;
class Sensor;
class Signal;

class IDeserialiser
{
public:
    virtual ~IDeserialiser() = default;

    /**
     * Deserialise the JSON at the given path into a Project.
     *
     * @param path Path of the JSON on the file-system.
     * @param worker A Worker capable of accepting additional work to facilitate the load (e.g. LoadSignalFileTask).
     * @return An owning container of the loaded Project.
     *
     * @throws std::runtime_error The JSON could not be deserialised.
     */
    virtual std::unique_ptr<Project> deserialise_project(
            const std::filesystem::path& path,
            Worker* worker
    ) = 0;
};

} // namespace echomap

#endif // ECHOMAP_IDESERIALISER_HPP
