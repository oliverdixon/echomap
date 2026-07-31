//
// Created by owd on 07/07/2026.
//

#ifndef ECHOMAP_CHANNELMAPPINGPANEL_HPP
#define ECHOMAP_CHANNELMAPPINGPANEL_HPP

#include <string>

#include "../objects/IDAllocator.hpp"
#include "IPanel.hpp"

namespace echomap
{

class IProjectMutationService;
class IProjectObserveService;
class IRenderInvalidateService;
class Signal;
class Sensor;
class WorkerResultDespatcher;

/**
 * Provides a panel for defining mappings between Signal and Sensor objects.
 */
class ChannelMappingPanel final : public IPanel
{
public:
    /**
     * Create a new ChannelMappingPanel to describe and configure Signal-Sensor mappings.
     *
     * @param mutation_service Service for mutating the active Project.
     * @param invalidator Service for forcing frame re-draws.
     * @param observer_service Service for observing the active Project state.
     */
    explicit ChannelMappingPanel(
            IProjectMutationService& mutation_service,
            IRenderInvalidateService& invalidator,
            const IProjectObserveService& observer_service
    );

    ChannelMappingPanel(const ChannelMappingPanel&) = delete;
    ChannelMappingPanel& operator=(const ChannelMappingPanel&) = delete;
    ChannelMappingPanel(ChannelMappingPanel&&) = delete;
    ChannelMappingPanel& operator=(ChannelMappingPanel&&) = delete;

    ~ChannelMappingPanel() override = default;

    [[nodiscard]] const char* get_imgui_name() const noexcept override;

    void draw() override;

    static const char* get_imgui_stable_name() noexcept;

private:
    struct AddChannelMappingRowCache
    {
        struct Item
        {
            id_type id;
            const char* name;
        };

        std::optional<Item> signal;
        std::optional<Item> sensor;
    };

    void draw_new_channel_mapping(const Project& active_project) noexcept;
    static void draw_existing_channel_mapping(const Project& active_project) noexcept;

    std::string panel_name;
    AddChannelMappingRowCache new_entry_cache;

    IProjectMutationService& mutation_service;
    IRenderInvalidateService& invalidator;
    const IProjectObserveService& observer_service;

    bool was_signal_combo_open = false; /**< Was the Associated Signal combo box open on the previous render cycle? */
    bool was_sensor_combo_open = false; /**< Was the Associated Sensor combo box open on the previous render cycle? */
};

} // namespace echomap

#endif // ECHOMAP_CHANNELMAPPINGPANEL_HPP
