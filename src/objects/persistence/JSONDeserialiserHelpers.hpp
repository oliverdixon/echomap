/**
 * @file
 *
 * JSONDeserialiserHelpers specification
 *
 * @author Oliver Dixon
 * @date 2026-07-25
 */

#ifndef ECHOMAP_JSONDESERIALISERHELPERS_HPP
#define ECHOMAP_JSONDESERIALISERHELPERS_HPP

#include <simdjson.h>

#include <unordered_map>

#include "../Sensor.hpp"
#include "../factories/SignalFactory.hpp"

namespace echomap
{

class Project;
class Signal;

// NOLINTBEGIN(*-assignment-in-if-condition)

/**
 * Provides a set of static helpers for parsing Project fields with SIMDJSON.
 *
 * @todo Document member functions and sequence points in the simdjson namespace.
 */
class JSONDeserialiserHelpers
{
public:
    template <typename simdjson_value> // NOLINT(*-identifier-naming)
    static simdjson::error_code get_root(
            simdjson::ondemand::object& root,
            simdjson_value& value
    )
    {
        auto error = value.get_object().get(root);
        if (error)
            return error;

        // Schema version check.
        static constexpr unsigned int expected_schema_version = 1;
        unsigned int actual_schema_version = 0;

        if ((error = root["schema_version"].get(actual_schema_version)))
            return error;

        if (actual_schema_version != expected_schema_version)
            throw std::runtime_error(
                    std::format(
                            "Project file was saved in an incompatible version {} (expected {}).",
                            actual_schema_version,
                            expected_schema_version
                    )
            );

        return simdjson::SUCCESS;
    }

    static simdjson::error_code get_metadata(
            simdjson::ondemand::object& root,
            Project& project
    );

    static simdjson::error_code get_sensors(
            simdjson::ondemand::object& root,
            Project& project,
            std::unordered_map<
                    std::string_view,
                    id_type>& loaded
    );

    static simdjson::error_code get_mappings(
            simdjson::ondemand::object& root,
            Project& project,
            const std::unordered_map<
                    std::string_view,
                    id_type>& signals,
            const std::unordered_map<
                    std::string_view,
                    id_type>& sensors
    );

    static void verify_sample_count(
            std::uint64_t reported_sample_count,
            const Signal& signal
    );

    template <typename ValueT>
    static simdjson::error_code get_or_default(
            simdjson::ondemand::object& obj,
            const std::string_view key,
            ValueT& out,
            const ValueT& fallback
    )
    {
        auto field = obj[key];

        if (field.error() == simdjson::NO_SUCH_FIELD) {
            out = fallback;
            return simdjson::SUCCESS;
        }

        return field.get(out);
    }

    static simdjson::error_code get_uniformly_sampled_signal_source(
            simdjson::ondemand::object& source,
            const SignalFactory& signal_factory
    );

    static simdjson::error_code get_variably_sampled_signal_source(
            simdjson::ondemand::object& source,
            const SignalFactory& signal_factory
    );
};

} // namespace echomap

namespace simdjson
{

template <typename simdjson_value>
auto tag_invoke(
        deserialize_tag /*unused*/,
        simdjson_value& value,
        echomap::SignalFactory& signal_factory
)
{
    ondemand::object root;
    auto error = value.get_object().get(root);
    if (error)
        return error;

    std::string_view name;
    if ((error = root["name"].get(name)))
        return error;
    signal_factory.set_signal_name(name);

    ondemand::object source;
    if ((error = root["source"].get_object().get(source)))
        return error;

    std::string_view kind;
    if ((error = source["kind"].get(kind)))
        return error;

    if (kind == "filesystem") {
        // Signals sourced from the file system are not loaded by the parser; we just populate the source information.

        std::string_view path;
        if ((error = source["path"].get(path)))
            return error;

        std::size_t channel_num;
        if ((error = source["channel"].get(channel_num)))
            return error;

        signal_factory.set_source(path, channel_num);
    } else if (kind == "embeddedUniform") {
        // Embedded signals with uniform sampling are constructed by the parser with the mandatory timing information.
        if ((error = echomap::JSONDeserialiserHelpers::get_uniformly_sampled_signal_source(source, signal_factory)))
            return error;
    } else if (kind == "embeddedVariable") {
        // Embedded signals with variable sampling are constructed by the parser with the optional timing information.
        if ((error = echomap::JSONDeserialiserHelpers::get_variably_sampled_signal_source(source, signal_factory)))
            return error;
    } else
        throw std::runtime_error(std::format("Signal {} specifies unknown source kind \"{}\".", name, kind));

    return SUCCESS;
}

template <typename simdjson_value>
static error_code tag_invoke(
        deserialize_tag /*unused*/,
        simdjson_value& value,
        echomap::Sensor& sensor
)
{
    ondemand::object root;
    auto error = value.get_object().get(root);
    if (error)
        return error;

    std::string_view name;
    if ((error = root["name"].get(name)))
        return error;
    sensor.set_name(name);

    ondemand::object position;
    if ((error = root["position"].get_object().get(position)))
        return error;

    if ((error = position["x"].get(sensor.position.x)))
        return error;
    if ((error = position["y"].get(sensor.position.y)))
        return error;
    if ((error = position["z"].get(sensor.position.z)))
        return error;

    ondemand::object colour;
    if ((error = root["colour"].get_object().get(colour)))
        return error;

    if ((error = colour["r"].get(sensor.colour.r)))
        return error;
    if ((error = colour["g"].get(sensor.colour.g)))
        return error;
    if ((error = colour["b"].get(sensor.colour.b)))
        return error;
    if ((error = colour["a"].get(sensor.colour.a)))
        return error;

    return SUCCESS;
}

// NOLINTEND(*-assignment-in-if-condition)

} // namespace simdjson

#endif // ECHOMAP_JSONDESERIALISERHELPERS_HPP
