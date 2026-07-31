/**
 * @file
 * @brief EchoMap class implementation
 * @author Oliver Dixon
 * @date 2026-05-05
 */

#include "EchoMap.hpp"

#include <sigc++/adaptors/bind.h>

#include "../async/tasks/LoadProjectTask.hpp"
#include "../controllers/ProjectControllerBase.hpp"
#include "../objects/Project.hpp"
#include "../objects/Signal.hpp"
#include "../panels/ChannelMappingPanel.hpp"
#include "../panels/MenuPanel.hpp"
#include "../panels/ProjectPanel.hpp"
#include "../panels/SensorGeometryPanel.hpp"
#include "../panels/SignalDFTPanel.hpp"
#include "../panels/SignalWaveformPanel.hpp"
#include "../utility/Logger.hpp"

namespace echomap
{

EchoMap::EchoMap(
        std::function<void()> worker_result_callback
) :
    worker(std::move(worker_result_callback))
{
}

EchoMap::~EchoMap() noexcept = default;

void EchoMap::setup_controller(
        std::unique_ptr<ProjectControllerBase> controller
)
{
    if (project_controller != nullptr)
        throw std::logic_error("A project controller already exists.");

    project_controller = std::move(controller);
    auto& p_cont = *this->project_controller; // Just an alias.

    panel_host.add_panel(std::make_unique<MenuPanel>(p_cont));
    panel_host.add_panel(std::make_unique<ProjectPanel>(p_cont));
    panel_host.add_panel(std::make_unique<SignalWaveformPanel>(worker, despatcher, p_cont));
    panel_host.add_panel(std::make_unique<SensorGeometryPanel>(p_cont, p_cont));
    panel_host.add_panel(std::make_unique<ChannelMappingPanel>(p_cont, render_host, p_cont));
    panel_host.add_panel(std::make_unique<SignalDFTPanel>(worker, despatcher, render_host, p_cont));

    // NOLINTBEGIN(*-redundant-casting) - False positive; casts are required for libsigcpp to resolve overloads.

    connections.emplace_back(despatcher.load_project_finished_channel.nominate_consumer(
            sigc::mem_fun(
                    *project_controller,
                    static_cast<void (ProjectControllerBase::*)(LoadProjectResult&&)>(
                            &ProjectControllerBase::handle_result
                    )
            )
    ));

    connections.emplace_back(despatcher.load_signal_file_channel.nominate_consumer(
            sigc::mem_fun(
                    *project_controller,
                    static_cast<void (ProjectControllerBase::*)(LoadSignalFileResult&&)>(
                            &ProjectControllerBase::handle_result
                    )
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
