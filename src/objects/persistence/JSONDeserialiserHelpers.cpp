/**
 * @file
 *
 * JSONDeserialiserHelpers implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-25
 */

#include "JSONDeserialiserHelpers.hpp"

#include "../Project.hpp"
#include "../Signal.hpp"

namespace echomap
{

simdjson::error_code JSONDeserialiserHelpers::get_metadata(
        simdjson::ondemand::object& root,
        Project& project
)
{
    simdjson::ondemand::object metadata;
    auto error = root["metadata"].get_object().get(metadata);
    if (error)
        return error;

    std::string project_name;
    if ((error = metadata["name"].get(project_name)))
        return error;

    project.set_name(project_name);
    return simdjson::SUCCESS;
}

simdjson::error_code JSONDeserialiserHelpers::get_sensors(
        simdjson::ondemand::object& root,
        Project& project,
        std::unordered_map<
                std::string_view,
                id_type>& loaded
)
{
    std::vector<std::unique_ptr<Sensor>> sensors;
    if (const auto error = root["sensors"].get(sensors))
        return error;

    for (auto&& sensor : sensors) {
        if (!loaded.emplace(sensor->get_name(), sensor->get_id()).second)
            throw std::runtime_error(std::format("Project contains duplicate sensor {}.", sensor->get_name()));
        project.add_sensor(std::move(sensor));
    }

    return simdjson::SUCCESS;
}

simdjson::error_code JSONDeserialiserHelpers::get_mappings(
        simdjson::ondemand::object& root,
        Project& project,
        const std::unordered_map<
                std::string_view,
                id_type>& signals,
        const std::unordered_map<
                std::string_view,
                id_type>& sensors
)
{
    simdjson::ondemand::array mappings;
    auto error = root["mappings"].get_array().get(mappings);
    if (error)
        return error;

    for (auto mapping : mappings) {
        simdjson::ondemand::object mapping_obj;
        if ((error = mapping.get_object().get(mapping_obj)))
            return error;

        std::string_view signal_name;
        std::string_view sensor_name;

        if ((error = mapping_obj["signal"].get(signal_name)))
            return error;

        if ((error = mapping_obj["sensor"].get(sensor_name)))
            return error;

        const auto signal_it = signals.find(signal_name);
        if (signal_it == signals.end())
            throw std::runtime_error(std::format("Channel mapping referred to non-existent signal {}.", signal_name));

        const auto sensor_it = sensors.find(sensor_name);
        if (sensor_it == sensors.end())
            throw std::runtime_error(std::format("Channel mapping referred to non-existent sensor {}.", sensor_name));

        project.add_association(signal_it->second, sensor_it->second);
    }

    return simdjson::SUCCESS;
}

void JSONDeserialiserHelpers::verify_sample_count(
        std::uint64_t reported_sample_count,
        const Signal& signal
)
{
    if (reported_sample_count != signal.get_sample_count())
        throw std::runtime_error(
                std::format(
                        "Embedded signal {} reported the incorrect number of samples: claimed {}, but received {}.",
                        signal.get_name(),
                        reported_sample_count,
                        signal.get_sample_count()
                )
        );
}

simdjson::error_code JSONDeserialiserHelpers::get_uniformly_sampled_signal_source(
        simdjson::ondemand::object& source,
        const SignalFactory& signal_factory
)
{
    std::uint64_t reported_sample_count;
    auto error = source["sample_count"].get(reported_sample_count);
    if (error)
        return error;

    std::size_t sample_rate;
    if ((error = source["sample_rate"].get(sample_rate)))
        return error;
    signal_factory.set_sample_rate(sample_rate);

    float time_offset;
    if ((error = source["time_offset"].get(time_offset)))
        return error;
    signal_factory.set_time_offset(time_offset);

    simdjson::ondemand::array samples;
    if ((error = source["samples"].get_array().get(samples)))
        return error;

    for (auto sample_wrapper : samples) {
        float amplitude;
        if ((error = sample_wrapper.get(amplitude)))
            return error;
        signal_factory.emplace_sample(amplitude);
    }

    verify_sample_count(reported_sample_count, signal_factory.observe_signal());
    return error;
}

simdjson::error_code JSONDeserialiserHelpers::get_variably_sampled_signal_source(
        simdjson::ondemand::object& source,
        const SignalFactory& signal_factory
)
{
    std::uint64_t reported_sample_count;
    auto error = source["sample_count"].get(reported_sample_count);
    if (error)
        return error;

    std::size_t sample_rate;
    if ((error = get_or_default(source, "sample_rate", sample_rate, std::size_t{0})))
        return error;
    signal_factory.set_sample_rate(sample_rate);

    float time_offset;
    if ((error = get_or_default(source, "time_offset", time_offset, 0.0f)))
        return error;
    signal_factory.set_time_offset(time_offset);

    simdjson::ondemand::array samples;
    if ((error = source["samples"].get_array().get(samples)))
        return error;

    for (auto sample_wrapper : samples) {
        simdjson::ondemand::object sample_obj;
        if ((error = sample_wrapper.get_object().get(sample_obj)))
            return error;
        Signal::Sample sample_data; // NOLINT(*-pro-type-member-init) - Immediately initialised.
        if ((error = sample_obj["time"].get(sample_data.time)))
            return error;
        if ((error = sample_obj["amplitude"].get(sample_data.amplitude)))
            return error;
        signal_factory.emplace_sample(sample_data.time, sample_data.amplitude);
    }

    verify_sample_count(reported_sample_count, signal_factory.observe_signal());
    return error;
}

} // namespace echomap
