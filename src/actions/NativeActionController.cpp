/**
 * @file
 *
 * NativeActionController implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-18
 */

#if !defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "NativeActionController.hpp"

namespace echomap
{

void NativeActionController::select_project_file_impl()
{
    notify<RaiseFileChooserNotification>(
        sigc::ptr_fun(&notify<ProjectSelectedNotification, const std::filesystem::path&>),
        sigc::ptr_fun(&notify<ProjectSelectedNotification>)
    );
}

void NativeActionController::register_vfs_mapping_impl(
        const std::size_t project_id,
        const std::filesystem::path& external
)
{
    std::ignore = project_id;
    std::ignore = external;

    throw std::runtime_error(
            "The NativeActionController received a request to query for VFS mappings, which are not applicable on the "
            "native platform."
    );
}

} // namespace echomap

#endif // __EMSCRIPTEN__
