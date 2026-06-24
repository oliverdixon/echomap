/**
 * @file
 * @brief WebCFD waveform view panel specification
 * @author Oliver Dixon
 * @date 2026-06-20
 */

#ifndef WEBCFD_PARAMETERSPANEL_HPP
#define WEBCFD_PARAMETERSPANEL_HPP

#include <implot.h>

#include <string>

#include "IPanel.hpp"
#include "audio/WAVData.hpp"

namespace WebCFD
{

/**
 * Defines a panel to display downsampled waveforms.
 */
class WaveformViewPanel final : public IPanel
{
public:
    explicit WaveformViewPanel(std::string panel_name, const AudioChannel& channel);

    [[nodiscard]] const char* get_imgui_name() const noexcept override;

    void draw() override;

private:
    const std::string panel_name;
    AudioChannel downsampled_channel;
    ImPlotSpec plotting_spec;
};

} // namespace WebCFD

#endif // WEBCFD_PARAMETERSPANEL_HPP
