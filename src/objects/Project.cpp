//
// Created by owd on 25/06/2026.
//

#include "Project.hpp"

#include <format>

#include "Sensor.hpp"
#include "Signal.hpp"
#include "factories/SignalFactory.hpp"

namespace echomap
{

void Project::indicate_unloaded_signal(
        std::unique_ptr<SignalFactory>&& factory
)
{
    if (!factory->observe_signal().observe_source().has_value())
        throw std::runtime_error("Attempted to defer load of a Signal with no external source.");

    const auto& source = *factory->observe_signal().observe_source();

    auto file_group_it = unloaded_signals.find(source.path);

    if (file_group_it == unloaded_signals.end()) {
        decltype(unloaded_signals)::mapped_type pair{{}, std::vector<std::unique_ptr<SignalFactory>>(source.channel)};
        const auto [it, success] = unloaded_signals.emplace(source.path, std::move(pair));

        if (!success)
            throw std::runtime_error("Could not register VFS channel mappings.");

        file_group_it = it;
    }

    auto& group_factories = file_group_it->second.second;

    if (group_factories.size() < source.channel)
        group_factories.resize(source.channel);

    group_factories[source.channel - 1] = std::move(factory);
}

void Project::add_vfs_mapping_for_unavailable_signal(
        const std::filesystem::path& external,
        std::filesystem::path&& internal
)
{
    const auto map_it = unloaded_signals.find(external);

    if (map_it == unloaded_signals.end())
        throw std::runtime_error(std::format("Provided VFS mapping for unknown path {}.", external.c_str()));

    map_it->second.first.emplace(std::move(internal));
}

Project::Project(
        const std::string_view project_name
) :
    Object(project_name)
{
}

Project::~Project() = default;

const Signal* Project::add_signal(
        std::shared_ptr<Signal>&& signal
)
{
    const auto [it, success] = signals.emplace(signal->get_id(), std::move(signal));
    if (!success)
        return nullptr;

    return it->second ? it->second.get() : nullptr;
}

const Sensor* Project::add_sensor(
        std::unique_ptr<Sensor>&& sensor
)
{
    const auto [it, success] = sensors.emplace(sensor->get_id(), std::move(sensor));
    if (!success)
        return nullptr;

    return it->second ? it->second.get() : nullptr;
}

void Project::add_association(
        const Signal& signal,
        const Sensor& sensor
)
{
    if (!signals.contains(signal.get_id()))
        throw std::runtime_error(std::format("{} does not exist in the project.", signal.get_name()));

    if (!sensors.contains(sensor.get_id()))
        throw std::runtime_error(std::format("{} does not exist in the project.", sensor.get_name()));

    if (!requested_channel_mapping.emplace(signal.get_id(), sensor.get_id()).second)
        throw std::runtime_error(
                std::format(
                        "Could not associate {} with {}: a component is already mapped.",
                        signal.get_name(),
                        sensor.get_name()
                )
        );
}

void Project::add_association(
        const id_type signal_id,
        const id_type sensor_id
)
{
    if (!requested_channel_mapping.emplace(signal_id, sensor_id).second)
        throw std::runtime_error(
                std::format(
                        "Could not associate Signal with ID {} to Sensor with ID {}: a component is already mapped.",
                        signal_id,
                        sensor_id
                )
        );
}

std::size_t Project::get_sensors_count() const noexcept
{
    return sensors.size();
}

ImPlot3DPoint Project::get_sensor_point(
        const int idx,
        const void* const project_instance
) noexcept
{
    const auto* const project_ptr = static_cast<const Project*>(project_instance);
    const auto [x, y, z] = project_ptr->sensors.values()[idx]->position;

    return { x, y, z };
}

Sensor& Project::get_mutable_sensor(
        const id_type sensor_id
)
{
    const auto sensor_it = sensors.find(sensor_id);
    if (sensor_it == sensors.end())
        throw std::runtime_error(std::format("No Sensor with ID {} belongs to {}.", sensor_id, get_name()));

    return *sensor_it->second;
}

std::optional<std::pair<
        const Signal&,
        const Sensor&>>
Project::resolve_pair(
        const id_type signal_id,
        const id_type sensor_id
) const
{
    /*
     * For the exception messages here, the best we can manage is the ID. Logging the display names would require
     * resolving the full Object, which we can't do if we can't find the thing!
     */

    const auto signal_it = signals.find(signal_id);
    if (signal_it == signals.end())
        return {};

    const auto sensor_it = sensors.find(sensor_id);
    if (sensor_it == sensors.end())
        return {};

    assert(signal_it->second != nullptr);
    assert(sensor_it->second != nullptr);

    return {{*signal_it->second, *sensor_it->second}};
}

} // namespace echomap
