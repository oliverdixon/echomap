/**
 * @file
 *
 * JSONPartialDeserialiser implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-25
 */

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "JSONPartialDeserialiser.hpp"

#include "../../factories/SignalFactory.hpp"
#include "../../Sensor.hpp"
#include "../../web/PartialProject.hpp"
#include "../JSONDeserialiserHelpers.hpp"

namespace
{

echomap::Worker* parent_worker = nullptr;

auto get_signals(
        simdjson::ondemand::object& root,
        echomap::PartialProject& project,
        std::unordered_map<
                std::string_view,
                echomap::id_type>& loaded
)
{
    // Step 1.  Create our factories which detain only the signal metadata (sample rate, etc.) and source information.
    std::vector<std::unique_ptr<echomap::SignalFactory>> factories;
    if (const auto error = root["signals"].get(factories))
        return error;

    /*
     * Step 2.  Process the signals. If they are embedded (no external source), they can be added to the Project
     * immediately. If they are external, we provide them to the PartialProject as unloaded signals, awaiting VFS
     * mappings.
     */
    for (auto factory : factories | std::views::as_rvalue) {
        const auto& signal = factory->observe_signal();

        if (!loaded.emplace(signal.get_name(), signal.get_id()).second)
            throw std::runtime_error(std::format("Project contains duplicate signal {}.", signal.get_name()));

        if (signal.observe_source().has_value())
            // File-system source: indicate to the PartialProject that the unloaded signal exists.
            project.indicate_unloaded_signal(std::move(factory));
        else
            // Embedded source: add it to the Project.
            project.add_signal(factory->take_signal());
    }

    return simdjson::SUCCESS;
}

}

namespace simdjson
{

template <typename simdjson_value>
auto tag_invoke(
        const deserialize_tag tag,
        simdjson_value& value,
        echomap::PartialProject& partial_project
)
{
    // NOLINTBEGIN(*-assignment-in-if-condition)

    std::ignore = tag;

    ondemand::object root;
    auto error = echomap::JSONDeserialiserHelpers::get_root(root, value);
    if (error)
        return error;

    // Metadata
    if ((error = echomap::JSONDeserialiserHelpers::get_metadata(root, partial_project)))
        return error;

    // Signals
    std::unordered_map<std::string_view, echomap::id_type> signal_ids;
    if ((error = get_signals(root, partial_project, signal_ids)))
        return error;

    // Sensors
    std::unordered_map<std::string_view, echomap::id_type> sensor_ids;
    if ((error = echomap::JSONDeserialiserHelpers::get_sensors(root, partial_project, sensor_ids)))
        return error;

    // Channel Map
    if ((error = echomap::JSONDeserialiserHelpers::get_mappings(root, partial_project, signal_ids, sensor_ids)))
        return error;

    return SUCCESS;

    // NOLINTEND(*-assignment-in-if-condition)
}

} // namespace simdjson

namespace echomap
{

std::unique_ptr<PartialProject> JSONPartialDeserialiser::deserialise_project(
        const std::filesystem::path& path,
        Worker* const worker
)
{
    parent_worker = worker;
    const auto json = simdjson::padded_string::load(path.c_str());
    auto doc = parser.iterate(json);
    auto partial_project = std::make_unique<PartialProject>();

    if (const auto error = doc.get(*partial_project); error)
        throw std::runtime_error(
                std::format(
                        "Could not load Project at {} due to error: {}",
                        path.c_str(),
                        simdjson::error_message(error)
                )
        );

    return partial_project;
}

} // namespace echomap

#endif // __EMSCRIPTEN__
