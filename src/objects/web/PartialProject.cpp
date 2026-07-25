/**
 * @file
 *
 * PartialProject implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-25
 */

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "PartialProject.hpp"

#include "../factories/SignalFactory.hpp"

namespace echomap
{

PartialProject::PartialProject(
        const std::string_view project_name
) :
    Project(project_name)
{
}

PartialProject::~PartialProject() = default;

void PartialProject::indicate_unloaded_signal(
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

void PartialProject::add_vfs_mapping_for_unavailable_signal(
        const std::filesystem::path& external,
        std::filesystem::path&& internal
)
{
    const auto map_it = unloaded_signals.find(external);

    if (map_it == unloaded_signals.end())
        throw std::runtime_error(std::format("Provided VFS mapping for unknown path {}.", external.c_str()));

    map_it->second.first.emplace(std::move(internal));
}

} // namespace echomap

#endif // __EMSCRIPTEN__
