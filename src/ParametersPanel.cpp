/**
 * @file
 * @brief WebCFD parameters ImGui panel implementation
 * @author Oliver Dixon
 * @date 2026-06-20
 */

#include "ParametersPanel.hpp"

#include <imgui.h>
#include <implot.h>

#include <ranges>

namespace WebCFD
{

ParametersPanel::ParametersPanel(
        std::function<void()> invalidate_layout_callback
) :
    invalidate_layout_callback(std::move(invalidate_layout_callback))
{
}

const char* ParametersPanel::get_imgui_name() const noexcept
{
    return panel_name.c_str();
}

void ParametersPanel::draw()
{
    ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;

    if (requires_repositioning || force_repositioning) {
        /*
         * If the reposition has been requested (or forced by the user), reset the ParametersPanel position to the
         * center of the overall viewport. Note that in the presence of Dear ImGui persistence (typically an 'imgui.ini'
         * file in the CWD, the position will not change unless the repositioning request was forced).
         */

        const ImGuiViewport* const viewport = ImGui::GetMainViewport();
        const ImVec2 center{
                viewport->WorkPos.x + viewport->WorkSize.x / 2.0f,
                viewport->WorkPos.y + viewport->WorkSize.y / 2.0f,
        };

        ImGui::SetNextWindowPos(
                center,
                force_repositioning ? ImGuiCond_Always : ImGuiCond_FirstUseEver,
                ImVec2{.5f, .5f}
        );

        flags |= ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize;

        requires_repositioning = false;
        force_repositioning = false;
    }

    ImPlotSpec spec;
    spec.Stride = sizeof(WAVData::AudioPoint);

    ImGui::Begin(panel_name.c_str(), nullptr, flags);

    std::size_t channel_idx = 0;

    for (const auto [original, downsample] : std::ranges::views::zip(wav_data, downsampled)) {
        if (ImPlot::BeginPlot(std::string("Audio Data: Channel " + std::to_string(channel_idx)).c_str())) {
            ImPlot::SetupAxes("PCM Value", "Time");
            ImPlot::PlotLine("Original Data", &original.front().time, &original.front().amplitude, static_cast<int>(original.size()), spec);
            ImPlot::PlotLine("Downsampled Data", &downsample.front().time, &downsample.front().amplitude, static_cast<int>(downsample.size()), spec);
            ImPlot::EndPlot();
        }

        ++channel_idx;
    }

    ImGui::End();
}

} // namespace WebCFD
