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

#include "IProjectPanel.hpp"

namespace echomap
{

class IProjectMutationService;
class Project;
class WorkerResultDespatcher;

/**
 * Provides a panel for defining and reviewing (in a 3D plot) positions of loaded Sensor objects in the active Project.
 */
class SensorGeometryPanel final : public IProjectPanel
{
public:
    /**
     * Create a new SensorGeometryPanel to plot and control Sensor information.
     *
     * @param mutation_service TODO
     * @param initial_project An optional initial Project for the IPanel to display.
     */
    explicit SensorGeometryPanel(
            IProjectMutationService& mutation_service,
            const Project* initial_project = nullptr
    );

    [[nodiscard]] const char* get_imgui_name() const noexcept override;

    void draw() noexcept override;

    void change_active_project(const Project* new_project) override;

    static const char* get_imgui_stable_name() noexcept;

private:
    void recache_sensor_colours() noexcept;
    void draw_geometry_summary() noexcept;
    void draw_geometry_plot() const noexcept;

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

    std::vector<ImU32> sensor_colours;
    ImPlot3DSpec plotting_spec_3d;
    const Project* active_project = nullptr;
    IProjectMutationService& mutation_service;
};

} // namespace echomap

#endif // ECHOMAP_SENSORGEOMETRYPANEL_HPP
