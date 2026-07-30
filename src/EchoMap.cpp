/**
 * @file
 * @brief EchoMap class implementation
 * @author Oliver Dixon
 * @date 2026-05-05
 */

#include "EchoMap.hpp"

#include <sigc++/adaptors/bind.h>

#include "async/tasks/LoadProjectTask.hpp"
#include "objects/Project.hpp"
#include "objects/Signal.hpp"
#include "panels/ChannelMappingPanel.hpp"
#include "panels/MenuPanel.hpp"
#include "panels/ProjectPanel.hpp"
#include "panels/SensorGeometryPanel.hpp"
#include "panels/SignalDFTPanel.hpp"
#include "panels/SignalWaveformPanel.hpp"
#include "utility/Logger.hpp"

#ifndef __EMSCRIPTEN__
#include "services/native/NativeProjectFilePicker.hpp"
#endif // __EMSCRIPTEN__

namespace echomap
{

EchoMap::EchoMap() :
    worker{[] {
#ifndef __EMSCRIPTEN__
        glfwPostEmptyEvent();
#endif
    }},
#ifdef __EMSCRIPTEN__
    project_controller(
            panel_host,
            worker
    )
#else
    project_controller(
            std::make_unique<NativeProjectFilePicker>(
                    panel_host,
                    render_host
            ),
            panel_host,
            worker
    )
#endif
{
    setup_subscriptions();

    panel_host.add_panel(std::make_unique<MenuPanel>(project_controller));
    panel_host.add_panel(std::make_unique<ProjectPanel>(project_controller));
    panel_host.add_panel(std::make_unique<SignalWaveformPanel>(&worker, despatcher, project_controller));
    panel_host.add_panel(std::make_unique<SignalDFTPanel>(&worker, despatcher, &render_host, project_controller));
    panel_host.add_panel(std::make_unique<SensorGeometryPanel>(project_controller, project_controller));
    panel_host.add_panel(std::make_unique<ChannelMappingPanel>(project_controller, render_host, project_controller));
}

EchoMap::~EchoMap() noexcept = default;

void EchoMap::tick()
{
    process_worker_results();
}

void EchoMap::setup_subscriptions()
{
    // NOLINTBEGIN(*-redundant-casting) - False positive; casts are required for libsigcpp to resolve overloads.

    connections.emplace_back(despatcher.load_project_finished_channel.nominate_consumer(
            sigc::mem_fun(
                    project_controller,
                    static_cast<void (ProjectController::*)(LoadProjectResult&&)>(&ProjectController::handle_result)
            )
    ));

    connections.emplace_back(despatcher.load_signal_file_channel.nominate_consumer(
            sigc::mem_fun(
                    project_controller,
                    static_cast<void (ProjectController::*)(LoadSignalFileResult&&)>(&ProjectController::handle_result)
            )
    ));

    // NOLINTEND(*-redundant-casting)

    connections.emplace_back(despatcher.error_channel.observe([this](const ErrorResult& error) {
        panel_host.raise_error(error.what());
        LOG_F_ERROR("Error modal raised due to error: {}", error.what());
    }));
}

void EchoMap::process_worker_results()
{
    while (auto result = worker.try_get_result())
        try {
            despatcher.publish(std::move(*result));
        } catch (const std::exception& exception) {
            Logger::log(Logger::Level::Error, exception.what(), std::source_location::current());
        }
}

} // namespace echomap
