/**
 * @file
 * @brief WebCFD parameters ImGui panel implementation
 * @author Oliver Dixon
 * @date 2026-06-20
 */

#include "ParametersPanel.hpp"

#include <imgui.h>
#include <implot.h>

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

    ImGui::Begin(panel_name.c_str(), nullptr, flags);

    if (ImPlot::BeginPlot("Audio Data")) {
        ImPlot::SetupAxes("PCM Value", "Time");
        ImPlot::PlotLine("v(t)", wav_data.get_data(), wav_data.get_frame_count());
        ImPlot::EndPlot();
    }

    ImGui::End();
}

} // namespace WebCFD
