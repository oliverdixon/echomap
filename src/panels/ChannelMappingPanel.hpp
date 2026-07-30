//
// Created by owd on 07/07/2026.
//

#ifndef ECHOMAP_CHANNELMAPPINGPANEL_HPP
#define ECHOMAP_CHANNELMAPPINGPANEL_HPP

#include <string>

#include "IProjectPanel.hpp"

namespace echomap
{

class IProjectMutationService;
class IRenderInvalidator;
class Signal;
class Sensor;
class WorkerResultDespatcher;

/**
 * Provides a panel for defining mappings between Signal and Sensor objects.
 */
class ChannelMappingPanel final : public IProjectPanel
{
public:
    /**
     * Create a new ChannelMappingPanel to describe and configure Signal-Sensor mappings.
     *
     * @param mutation_service TODO
     * @param invalidator TODO
     * @param initial_project An optional initial Project for the IPanel to display.
     */
    explicit ChannelMappingPanel(
            IProjectMutationService& mutation_service,
            IRenderInvalidator& invalidator,
            const Project* initial_project = nullptr
    );

    [[nodiscard]] const char* get_imgui_name() const noexcept override;

    void draw() noexcept override;

    void change_active_project(const Project* new_project) override;

    static const char* get_imgui_stable_name() noexcept;

private:
    void draw_new_channel_mapping() noexcept;
    void draw_existing_channel_mapping() const noexcept;

    std::string panel_name;

    struct AddChannelMappingRowCache
    {
        const Signal * signal = nullptr;
        const Sensor * sensor = nullptr;
    } new_entry_cache;

    IProjectMutationService& mutation_service;
    const Project* active_project = nullptr; // TODO remove.
    IRenderInvalidator& invalidator;

    bool was_signal_combo_open = false; /**< Was the Associated Signal combo box open on the previous render cycle? */
    bool was_sensor_combo_open = false; /**< Was the Associated Sensor combo box open on the previous render cycle? */
};

} // namespace echomap

#endif // ECHOMAP_CHANNELMAPPINGPANEL_HPP
