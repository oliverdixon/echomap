/**
 * @file
 *
 * PartialProject specification
 *
 * @author Oliver Dixon
 * @date 2026-07-25
 */

#ifndef ECHOMAP_PARTIALPROJECT_HPP
#define ECHOMAP_PARTIALPROJECT_HPP

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include <filesystem>
#include <map>

#include "../Project.hpp"
#include "../factories/SignalFactory.hpp"

namespace echomap
{

/**
 * A Project with semantics for expressing an incomplete set of owned Signal objects.
 *
 * In addition to the regular Project, the PartialProject detains a number of SignalFactory objects that are reserved
 * to construct Signal instances once their external sources become available. This is useful in WebAssembly
 * environments to model mappings between the Signal path specified in the Project metadata, and a mapping to the Wasm
 * VFS path.
 */
class PartialProject : public Project
{
    /**
     * Provides a mapping between stated paths of externally sourced signals, and paths in the WebAssembly VFS.
     *
     * The key indicates the path of the referenced file. The value composes an optional mapping of the corresponding
     * path in the VFS and a channel map of factories responsible for constructing the Signal of the file channel.
     */
    std::map<
            std::filesystem::path,
            std::pair<std::optional<std::filesystem::path>, std::vector<std::unique_ptr<SignalFactory>>>>
            unloaded_signals;

public:
    explicit PartialProject(std::string_view project_name = {});

    ~PartialProject() override;

    /**
     * Provide the factory for an unloaded Signal.
     *
     * An unloaded Signal is an externally sourced Signal defined by the Project, but whose sample data was not
     * available at the time of the Project load, and has not yet been fully loaded.
     *
     * @param factory The SignalFactory prepared to construct the Signal, once the data becomes available.
     *
     * @throws std::runtime_error The given SignalFactory did not define an external source.
     * @throws std::runtime_error Could not take ownership of the SignalFactory for some implementation-defined reason.
     * @throws std::invalid_argument The Signal channel was zero, where channel numbers are one-based.
     */
    void indicate_unloaded_signal(std::unique_ptr<SignalFactory>&& factory);

    /**
     * Provide a VFS mapping for a previously indicated unloaded Signal.
     *
     * @param external The external source path of the Signal.
     * @param internal The VFS-mapped path to use for the Signal.
     *
     * @throws std::runtime_error There was no unloaded Signal sourced by the given external path.
     */
    void add_vfs_mapping_for_unavailable_signal(
            const std::filesystem::path& external,
            std::filesystem::path&& internal
    );

    /**
     * Provide an observing view to the unloaded signals.
     *
     * @return An observing view of the unloaded signals structure including:
     *  - Key: the external path;
     *  - Value:
     *      -# Optionally, a VFS mapping to the internal path of the Signal source;
     *      -# The SignalFactory responsible for constructing the Signal once its data becomes available.
     */
    [[nodiscard]] auto observe_unloaded_signals() const noexcept
    {
        return unloaded_signals | std::views::all;
    }

    /**
     * Determines if all unloaded SignalFactory objects have a VFS mapping.
     *
     * @return Do all unloaded SignalFactory objects have a VFS mapping?
     */
    [[nodiscard]] bool all_sources_mapped() const noexcept;

    /**
     * Provide a view to the mutable unloaded SignalFactory objects.
     *
     * For a description of the viewed values, see @ref observe_unloaded_signals. Following this operation, ownership of
     * the unloaded factories are transferred to the caller and their corresponding entries removed from the underlying
     * storage.
     *
     * @return A mutating view of the unloaded Signal SignalFactory objects.
     */
    [[nodiscard]] auto drain_unloaded_factories() noexcept
    {
        return std::exchange(unloaded_signals, {}) | std::views::values | std::views::as_rvalue;
    }
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_PARTIALPROJECT_HPP
