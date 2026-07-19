/**
 * @file
 *
 * WaveFilePreloadNotification specification
 *
 * @author Oliver Dixon
 * @date 2026-07-19
 */

#ifndef ECHOMAP_WAVEFILEPRELOADNOTIFICATION_HPP
#define ECHOMAP_WAVEFILEPRELOADNOTIFICATION_HPP

#include "../../objects/Project.hpp"
#include "../../objects/Signal.hpp"

namespace echomap
{

/**
 * Indicates that the pre-load path for a Signal source has changed.
 * @todo Document better.
 */
struct WaveFilePreloadNotification
{
    explicit WaveFilePreloadNotification(
            const Project::id_type project_id,
            const Signal::id_type signal_id,
            std::filesystem::path path
    ) :
        project_id(project_id),
        signal_id(signal_id),
        path(std::move(path))
    {
    }

    Project::id_type project_id;
    Signal::id_type signal_id;
    std::filesystem::path path;
};

} // namespace echomap

#endif // ECHOMAP_WAVEFILEPRELOADNOTIFICATION_HPP
