/**
 * @file
 * @brief EchoMap signal waveform preview panel implementation
 * @author Oliver Dixon
 * @date 2026-07-07
 */

#include "SignalWaveformPanel.hpp"

#include "../objects/Project.hpp"
#include "../objects/SignalFactory.hpp"

namespace echomap
{

SignalWaveformPanel::SignalWaveformPanel(
        Project* const initial_project
) :
    active_project(initial_project)
{
}

const char* SignalWaveformPanel::get_imgui_name() const noexcept
{
    return panel_name.c_str();
}

void SignalWaveformPanel::draw() noexcept
{
    if (ImGui::Begin(panel_name.c_str())) {
        if (active_project == nullptr)
            ImGui::Text("No project is loaded.");
        else if (active_project->get_signal_count() > 0 && ImPlot::BeginAlignedPlots("##WaveformAlignedGroup")) {
            ImPlot::PushStyleColor(ImPlotCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

            for (const auto& signal : active_project->observe_signals())
                if (const auto downsampled = get_downsampled_signal(signal); downsampled == nullptr)
                    ImGui::Text("Could not downsample %s due to system error.", signal.get_imgui_name());
                else if (ImPlot::BeginPlot(downsampled->get_imgui_name())) {

                    ImPlot::SetupAxes("Time (seconds)", "Amplitude");
                    ImPlot::SetupAxisLinks(ImAxis_X1, &waveform_bounding_box.X.Min, &waveform_bounding_box.X.Max);
                    ImPlot::SetupAxisLinks(ImAxis_Y1, &waveform_bounding_box.Y.Min, &waveform_bounding_box.Y.Max);

                    CallbackData callback_data = {.signal = downsampled};
                    ImPlot::PlotLineG(
                            "",
                            &SignalWaveformPanel::get_indexed_signal_point,
                            &callback_data,
                            static_cast<int>(downsampled->get_sample_count()),
                            plotting_spec_2d
                    );

                    ImPlot::EndPlot();
                }

            ImPlot::EndAlignedPlots();
            ImPlot::PopStyleColor();
        }
    }

    ImGui::End();
}

void SignalWaveformPanel::set_active_project(
        Project* const new_active_project
) noexcept
{
    active_project = new_active_project;

    waveform_bounding_box.X.Min = std::numeric_limits<double>::max();
    waveform_bounding_box.X.Max = std::numeric_limits<double>::lowest();
    waveform_bounding_box.Y.Min = -1.0;
    waveform_bounding_box.Y.Max = 1.0;
}

ImPlotPoint SignalWaveformPanel::get_indexed_signal_point(
        const int index,
        // ReSharper disable once CppParameterMayBeConstPtrOrRef - Signature enforced by ImPlot.
        void* const user_data
) noexcept
{
    const auto signal = static_cast<CallbackData*>(user_data)->signal;

    return {signal->get_time_at_index(index), signal->begin()[index]};
}

const Signal* SignalWaveformPanel::get_downsampled_signal(
        const Signal& signal
)
{
    auto downsampled_it = downsample_cache.find(signal.get_id());
    bool success;

    if (downsampled_it == downsample_cache.end()) {

        // If the source signal hasn't already been downsampled and cached, do it now.

        auto downsampled = SignalFactory::downsample(signal, default_downsample_factor);
        auto [added_it, was_added] = downsample_cache.emplace(signal.get_id(), std::move(downsampled));

        downsampled_it = added_it;
        success = was_added;

        if (success) {
            // Update the horizontal extent of the bounding box of the downsampled signals.
            const auto& ds_signal = *added_it->second;
            const auto local_min = ds_signal.get_time_at_index(0);
            const auto local_max = ds_signal.get_time_at_index(ds_signal.get_sample_count() - 1);

            if (local_min < waveform_bounding_box.X.Min)
                waveform_bounding_box.X.Min = local_min;

            if (local_max > waveform_bounding_box.X.Max)
                waveform_bounding_box.X.Max = local_max;
        }
    } else
        success = true;

    return success ? downsampled_it->second.get() : nullptr;
}

} // namespace echomap
