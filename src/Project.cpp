//
// Created by owd on 25/06/2026.
//

#include "Project.hpp"

#include <format>
#include <ranges>

namespace WebCFD
{

Project::Project(
        const std::string_view project_name
) :
    project_name(project_name)
{
    // TODO remove. Adding data for testing...

    add_signal(std::make_unique<Signal>());
    add_signal(std::make_unique<Signal>());
    add_signal(std::make_unique<Signal>("Named signal"));

    add_sensor(std::make_unique<Sensor>("Sensor A"));
    add_sensor(std::make_unique<Sensor>("Sensor B"));
    add_sensor(std::make_unique<Sensor>());
}

void Project::add_signal(
        std::unique_ptr<Signal>&& signal
)
{
    signals.emplace(signal->get_id(), std::move(signal));
}

void Project::add_sensor(
        std::unique_ptr<Sensor>&& sensor
)
{
    sensors.emplace(sensor->get_id(), std::move(sensor));
}

void Project::add_association(
        const Signal& signal,
        const Sensor& sensor
)
{
    if (!signals.contains(signal.get_id()))
        throw std::runtime_error(std::format("Signal with ID {} does not exist in the project.", signal.get_id()));

    if (!sensors.contains(sensor.get_id()))
        throw std::runtime_error(std::format("Sensor with ID {} does not exist in the project.", sensor.get_id()));

    channel_mapping.emplace(signal.get_id(), sensor.get_id());
}

bool Project::are_signals_stored() const noexcept
{
    return !signals.empty();
}

bool Project::are_sensors_stored() const noexcept
{
    return !sensors.empty();
}

} // namespace WebCFD
