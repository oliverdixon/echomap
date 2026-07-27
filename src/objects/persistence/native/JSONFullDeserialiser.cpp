//
// Created by owd on 07/07/2026.

#if !defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "JSONFullDeserialiser.hpp"

#include "../../../async/Worker.hpp"
#include "../../../async/tasks/LoadSignalFileTask.hpp"
#include "../../Project.hpp"
#include "../../Signal.hpp"
#include "../../factories/SignalFactory.hpp"
#include "../JSONDeserialiserHelpers.hpp"

/**
 * Free helper functions for simdjson customisation points.
 */
namespace
{

echomap::Worker* parent_worker = nullptr;

auto get_signals(
        simdjson::ondemand::object& root,
        echomap::Project& project,
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
     * Step 2.  Process the signals. Embedded signals (no external source) can be added to the Project immediately.
     * Externally sourced signals are prepared in the slots map, from which LoadSignalFileTasks can be scheduled.
     *
     * The key of the map is the source filename, and the value is a bijective correspondence between the channel in the
     * wave file and the factory designated to load the channel at the index.
     *
     * That is, the first slot owns the factory for constructing the signal on Channel 1, the second for Channel 2, etc.
     * If there is a gap, the entry is the empty unique_ptr<SignalFactory>, such that it equals nullptr.
     */
    std::unordered_map<std::string, std::vector<std::unique_ptr<echomap::SignalFactory>>> slots;

    for (auto factory : factories | std::views::as_rvalue) {
        const auto& signal = factory->observe_signal();
        if (!loaded.emplace(signal.get_name(), signal.get_id()).second)
            throw std::runtime_error(std::format("Project contains duplicate signal {}.", signal.get_name()));

        if (signal.observe_source().has_value()) {
            // File-system source: prepare its factory for LoadSignalFileTask.
            auto& slot_vector = slots[signal.observe_source()->path.c_str()];
            const auto channel_num = signal.observe_source()->channel;

            // Reminder: channel numbers are 1-based.
            if (channel_num > slot_vector.size())
                slot_vector.resize(channel_num);
            else if (slot_vector[channel_num - 1] != nullptr)
                throw std::runtime_error(
                        std::format(
                                "Both signals {} and {} have requested the same channel {} from {}.",
                                slot_vector[channel_num - 1]->observe_signal().get_name(),
                                signal.get_name(),
                                channel_num,
                                signal.observe_source()->path.c_str()
                        )
                );

            slot_vector[channel_num - 1] = std::move(factory);
        } else {
            // Embedded source: add it to the Project.
            auto& stolen = *std::move(factory);
            project.add_signal(std::move(stolen).take_signal());
        }
    }

    /*
     * Step 3.  Once we have the factories grouped by source, and put into the bijective correspondence with the channel
     * numbers, submit a task to the thread-safe Worker for each distinct file. The task takes ownership of the
     * factories.
     *
     * The results will own the signal objects produced by the factories, which can be added to the project by the
     * EchoMap controller (or whoever is the nominated consumer).
     */
    if (!slots.empty()) {
        if (parent_worker == nullptr)
            throw std::runtime_error(
                    std::format(
                            "Need to despatch extra work to load {}, but a suitable Worker is unavailable.",
                            project.get_name()
                    )
            );

        for (auto&& [file_path, slot_vector] : slots)
            parent_worker->submit(
                    std::make_unique<echomap::LoadSignalFileTask>(project.get_id(), file_path, std::move(slot_vector))
            );
    }

    return simdjson::SUCCESS;
}

} // namespace

namespace simdjson
{

template <typename simdjson_value>
auto tag_invoke(
        const deserialize_tag tag,
        simdjson_value& value,
        echomap::Project& project
)
{
    // NOLINTBEGIN(*-assignment-in-if-condition)

    std::ignore = tag;

    ondemand::object root;
    auto error = echomap::JSONDeserialiserHelpers::get_root(root, value);
    if (error)
        return error;

    // Metadata
    if ((error = echomap::JSONDeserialiserHelpers::get_metadata(root, project)))
        return error;

    // Signals
    std::unordered_map<std::string_view, echomap::id_type> signal_ids;
    if ((error = get_signals(root, project, signal_ids)))
        return error;

    // Sensors
    std::unordered_map<std::string_view, echomap::id_type> sensor_ids;
    if ((error = echomap::JSONDeserialiserHelpers::get_sensors(root, project, sensor_ids)))
        return error;

    // Channel Map
    if ((error = echomap::JSONDeserialiserHelpers::get_mappings(root, project, signal_ids, sensor_ids)))
        return error;

    return SUCCESS;

    // NOLINTEND(*-assignment-in-if-condition)
}

}

namespace echomap
{

std::unique_ptr<Project> JSONFullDeserialiser::deserialise_project(
        const std::filesystem::path& path,
        Worker* const worker
)
{
    parent_worker = worker;
    const auto json = simdjson::padded_string::load(path.c_str());
    auto doc = parser.iterate(json);
    auto project = std::make_unique<Project>();

    if (const auto error = doc.get(*project); error)
        throw std::runtime_error(
                std::format(
                        "Could not load Project at {} due to error: {}",
                        path.c_str(),
                        simdjson::error_message(error)
                )
        );

    return project;
}

} // namespace echomap

#endif // __EMSCRIPTEN__
