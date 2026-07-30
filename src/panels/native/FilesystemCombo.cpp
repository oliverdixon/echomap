/**
 * @file
 *
 * FilesystemCombo implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-23
 */

#if !defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "FilesystemCombo.hpp"

#include <imgui.h>

#include <algorithm>
#include <cstring>

#include "../../platform/EchoMap.hpp"

namespace echomap
{

FilesystemCombo::FilesystemCombo(
        IRenderInvalidator& invalidator
) :
    invalidator(invalidator)
{
    std::error_code error_code;
    const auto cwd = std::filesystem::current_path(error_code);

    if (error_code || cwd.empty() || !std::filesystem::is_directory(cwd))
        throw std::runtime_error("Could not get the current working directory. Is the filesystem readable?");

    update_current_state(cwd);
}

bool FilesystemCombo::operator()(
        std::filesystem::path& selected_path
)
{
    bool changed = false;

    ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());

    const std::string preview = selected_path.empty() ? std::string{current_root.data()} : selected_path.string();
    const auto is_combo_open = ImGui::BeginCombo("##FilesystemCombo", preview.c_str(), ImGuiComboFlags_HeightLarge);

    if (is_combo_open) {
        if (ImGui::IsWindowAppearing()) {
            ImGui::SetKeyboardFocusHere();
            invalidate_cache();
        }

        changed = draw_combo_body(selected_path);

        ImGui::EndCombo();

        if (!was_combo_open)
            invalidator.force_frames();
    }

    was_combo_open = is_combo_open;

    return changed;
}

FilesystemCombo::Entry::Entry(
        const std::filesystem::path& path,
        const bool is_directory_hint
) :
    path(path),
    name(path.filename().string()),
    is_directory(is_directory_hint),
    imgui_id((is_directory ? name + '/' : name) + "##" + path.string()),
    flags(is_directory ? ImGuiSelectableFlags_NoAutoClosePopups : ImGuiSelectableFlags_None)
{
    assert(is_directory == std::filesystem::is_directory(path));
}

bool FilesystemCombo::Entry::draw() const noexcept
{
    return ImGui::Selectable(imgui_id.c_str(), false, flags);
}

void FilesystemCombo::invalidate_cache()
{
    cache.reset();
}

FilesystemCombo::EntryCache::EntryCache(
        const std::filesystem::path& directory
) :
    directory(directory)
{
    std::error_code error_code;

    std::filesystem::directory_iterator iterator{
            directory,
            std::filesystem::directory_options::skip_permission_denied |
                    std::filesystem::directory_options::follow_directory_symlink,
            error_code
    };

    const std::filesystem::directory_iterator end{};

    while (!error_code && iterator != end) {
        const std::filesystem::directory_entry& directory_entry = *iterator;
        std::error_code entry_error_code;
        const std::filesystem::file_status status = directory_entry.status(entry_error_code);

        if (!entry_error_code) {
            // ReSharper disable CppTooWideScopeInitStatement
            const bool is_directory = std::filesystem::is_directory(status);
            const bool is_regular_file = std::filesystem::is_regular_file(status);
            // ReSharper restore CppTooWideScopeInitStatement

            if (is_directory || is_regular_file)
                entries.emplace_back(directory_entry.path(), is_directory);
        }

        iterator.increment(error_code);
    }

    std::ranges::sort(entries);
}

void FilesystemCombo::update_current_state(
        const std::filesystem::path& path
)
{
    const std::string path_string = path.string();
    const std::size_t length = std::min(current_root.size() - 1, path_string.size());

    std::memcpy(current_root.data(), path_string.data(), length);
    current_root[length] = '\0';

    current_target = get_browse_target();
}

bool FilesystemCombo::draw_combo_body(
        std::filesystem::path& selected_path
)
{
    ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
    const std::string old_root = current_root.data();
    const bool enter_pressed =
            ImGui::InputText("##path", current_root.data(), current_root.size(), ImGuiInputTextFlags_EnterReturnsTrue);

    bool changed = false;

    if (old_root != current_root.data())
        // If the browse target has changed, update the cached member variable.
        current_target = get_browse_target();

    if (enter_pressed) {
        // If the user has explicitly pressed 'enter', accept whatever is selected at this time.
        accept_path(
                selected_path,
                current_target.target_path.empty() ? current_target.parent_directory : current_target.target_path
        );

        changed = true;
        ImGui::CloseCurrentPopup();
    }

    ImGui::Separator();

    if (!std::filesystem::is_directory(current_target.parent_directory))
        ImGui::TextDisabled("Not a readable directory.");
    else {
        changed |= draw_parent_entry(current_target.parent_directory);
        if (!cache || cache->directory != current_target.parent_directory)
            cache.emplace(current_target.parent_directory);

        changed |= draw_child_entries(current_target.filter, selected_path);
    }

    return changed;
}

FilesystemCombo::BrowseTarget FilesystemCombo::get_browse_target() const
{
    std::error_code error_code;
    const std::string typed_string = current_root.data();
    auto const typed_path = typed_string.empty() ? std::filesystem::path{} : std::filesystem::path{typed_string};
    auto lookup_path = typed_path;

    if (lookup_path.empty())
        lookup_path = std::filesystem::current_path(error_code);
    else if (lookup_path.is_relative())
        lookup_path = std::filesystem::absolute(lookup_path, error_code);

    if (error_code || lookup_path.empty())
        lookup_path = ".";

    error_code.clear();

    if (std::filesystem::is_directory(lookup_path, error_code))
        return {.target_path = lookup_path, .parent_directory = lookup_path, .filter = {}};

    std::filesystem::path directory = lookup_path.parent_path();

    if (directory.empty()) {
        error_code.clear();
        directory = std::filesystem::current_path(error_code);

        if (error_code || directory.empty())
            directory = ".";
    }

    return {.target_path = lookup_path, .parent_directory = directory, .filter = lookup_path.filename().string()};
}

bool FilesystemCombo::draw_parent_entry(
        const std::filesystem::path& directory
)
{
    const std::filesystem::path parent = directory.parent_path();

    if (parent.empty() || parent == directory)
        return false;

    if (ImGui::Selectable("../", false, ImGuiSelectableFlags_NoAutoClosePopups)) {
        update_current_state(parent);
        return true;
    }

    return false;
}

bool FilesystemCombo::draw_child_entries(
        const std::string_view filter,
        std::filesystem::path& selected_path
)
{
    assert(cache.has_value());

    bool has_visible_entries = false;
    bool changed = false;

    for (const auto& entry : cache->entries) {
        if (!filter.empty() && !entry.name.contains(filter))
            continue;

        has_visible_entries = true;

        if (!entry.draw())
            continue;

        if (entry.is_directory) {
            update_current_state(entry.path);
            return false;
        }

        selected_path = entry.path;
        update_current_state(entry.path.parent_path());

        changed = true;
        ImGui::CloseCurrentPopup();

        return changed;
    }

    if (!has_visible_entries)
        ImGui::TextDisabled("No matching entries.");

    return changed;
}

void FilesystemCombo::accept_path(
        std::filesystem::path& selected_path,
        const std::filesystem::path& path
)
{
    if (std::filesystem::is_directory(path)) {
        update_current_state(path);
        return;
    }

    selected_path = path;

    if (path.has_parent_path())
        update_current_state(path.parent_path());
    else
        update_current_state(".");
}

} // namespace echomap

#endif // __EMSCRIPTEN__
