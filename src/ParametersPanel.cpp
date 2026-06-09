//
// Created by owd on 04/06/2026.
//

#include "ParametersPanel.hpp"

#include <imgui.h>

namespace WebCFD
{

ParametersPanel::ParametersPanel(
        SimulationParameters& parameters
) :
    parameters(parameters)
{
}

void ParametersPanel::draw()
{
    ImGui::Begin("Simulation Parameters");

    ImGui::SliderFloat("Speed", &parameters.controls.x, 0.0f, 4.0f);
    ImGui::SliderFloat("Intensity", &parameters.controls.y, 0.0f, 3.0f);
    ImGui::SliderFloat("Warp", &parameters.controls.z, 0.0f, 3.0f);
    ImGui::SliderFloat("Scale", &parameters.controls.w, 0.25f, 4.0f);

    ImGui::ColorEdit3("Colour A", &parameters.colour_a.x);
    ImGui::ColorEdit3("Colour B", &parameters.colour_b.x);
    ImGui::ColorEdit3("Colour C", &parameters.colour_c.x);
    ImGui::ColorEdit3("Colour D", &parameters.colour_d.x);

    ImGui::End();
}

} // namespace WebCFD
