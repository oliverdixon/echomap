/**
 * @file
 * @brief WebCFD parameters ImGui panel implementation
 * @author Oliver Dixon
 * @date 2026-06-20
 */

#include "WaveformViewPanel.hpp"

#include <imgui.h>
#include <implot.h>

#include <ranges>

#include "../Logger.hpp"

namespace WebCFD
{

WaveformViewPanel::WaveformViewPanel(
        std::string panel_name,
        const AudioChannel& channel
) :
    panel_name(std::move(panel_name)),
    downsampled_channel(channel, 50.0f)
{
    plotting_spec.Stride = sizeof(AudioChannel::Sample);
}

const char* WaveformViewPanel::get_imgui_name() const noexcept
{
    return panel_name.c_str();
}

void WaveformViewPanel::draw()
{
    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
    ImGui::Begin(panel_name.c_str(), nullptr, flags);

    if (downsampled_channel.get_sample_count() > 0 && ImPlot::BeginPlot("Waveform")) {
        ImPlot::SetupAxes("Amplitude", "Time");

        ImPlot::PlotLine(
                "",
                &downsampled_channel.begin()->time,
                &downsampled_channel.begin()->amplitude,
                static_cast<int>(downsampled_channel.get_sample_count()),
                plotting_spec
        );

        ImPlot::EndPlot();
    }

    ImGui::End();
}

} // namespace WebCFD
