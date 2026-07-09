//
// Created by owd on 07/07/2026.
//

#ifndef ECHOMAP_CHANNELMAPPINGPANEL_HPP
#define ECHOMAP_CHANNELMAPPINGPANEL_HPP

#include "ErrorModal.hpp"
#include "IPanel.hpp"

namespace echomap
{

class Signal;
class Sensor;

/**
 * Provides a panel for defining mappings between Signal and Sensor objects.
 */
class ChannelMappingPanel final : public IPanel
{
public:
    explicit ChannelMappingPanel(Project* initial_project = nullptr);

    [[nodiscard]] const char* get_imgui_name() const noexcept override;

    void draw() noexcept override;

    void set_active_project(Project* new_active_project) noexcept override;

private:
    void draw_new_channel_mapping() noexcept;
    void draw_existing_channel_mapping() const noexcept;

    const std::string panel_name = "Channel Mapping";

    struct AddChannelMappingRowCache
    {
        const Signal * selected_signal = nullptr;
        const Sensor * selected_sensor = nullptr;
    } new_mapping_cache;

    ErrorModal error_modal;
    Project* active_project = nullptr;
};

} // namespace echomap

#endif // ECHOMAP_CHANNELMAPPINGPANEL_HPP
