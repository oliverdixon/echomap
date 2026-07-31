/**
 * @file
 * @brief EchoMap sensor geometry panel specification
 * @author Oliver Dixon
 * @date 2026-07-07
 */

#ifndef ECHOMAP_SENSORGEOMETRYPANEL_HPP
#define ECHOMAP_SENSORGEOMETRYPANEL_HPP

#include <implot3d.h>

#include <string>

#include "IPanel.hpp"
#include "../objects/IDAllocator.hpp"

namespace echomap
{

class IProjectMutationService;
class IProjectObserveService;

/**
 * Provides a panel for defining and reviewing (in a 3D plot) positions of loaded Sensor objects in the active Project.
 */
class SensorGeometryPanel final : public IPanel
{
public:
    /**
     * Create a new SensorGeometryPanel to plot and control Sensor information.
     *
     * @param mutation_service Service for performing simple mutations on the active Project.
     * @param observer_service Service for observing the active Project.
     */
    explicit SensorGeometryPanel(
            IProjectMutationService& mutation_service,
            const IProjectObserveService& observer_service
    );

    SensorGeometryPanel(const SensorGeometryPanel&) = delete;
    SensorGeometryPanel& operator=(const SensorGeometryPanel&) = delete;
    SensorGeometryPanel(SensorGeometryPanel&&) = delete;
    SensorGeometryPanel& operator=(SensorGeometryPanel&&) = delete;

    ~SensorGeometryPanel() override = default;

    [[nodiscard]] const char* get_imgui_name() const noexcept override;

    void draw() noexcept override;

    static const char* get_imgui_stable_name() noexcept;

private:
    void recache_sensor_colours(const Project& active_project) noexcept;
    void draw_geometry_summary(const Project& active_project) noexcept;
    void draw_geometry_plot(const Project& active_project) const noexcept;

    /**
     * Callback helper to get the ImPlot3DPoint representation of the stored Sensor at a fixed index.
     *
     * @param idx The zero-based index of the stored Sensor to query.
     * @param project_instance An observing pointer to the Project instance which stores the Sensor to query.
     * @return The position of the Sensor stored by the Project at the index.
     *
     * @pre The index must be less than the number of stored Sensor objects in the Project.
     */
    [[nodiscard]] static ImPlot3DPoint get_sensor_point(
            int idx,
            const void* project_instance
    ) noexcept;

    std::string panel_name;

    std::optional<id_type> cached_project_id;
    std::vector<ImU32> sensor_colours;
    ImPlot3DSpec plotting_spec_3d;

    IProjectMutationService& mutation_service;
    const IProjectObserveService& observer_service;
};

} // namespace echomap

#endif // ECHOMAP_SENSORGEOMETRYPANEL_HPP
