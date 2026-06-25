//
// Created by owd on 25/06/2026.
//

#ifndef WEBCFD_PROJECT_HPP
#define WEBCFD_PROJECT_HPP

#include <memory>
#include <ranges>
#include <string>
#include <string_view>

#include "BidirectionalUnorderedMapping.hpp"
#include "audio/Sensor.hpp"
#include "audio/WAVData.hpp"

namespace WebCFD
{

class Project
{
public:
    explicit Project(std::string_view project_name);

    void add_signal(std::unique_ptr<Signal>&& signal);

    void add_sensor(std::unique_ptr<Sensor>&& sensor);

    /**
     * Creates a new association between a signal and sensor.
     *
     * @param signal The associated signal
     * @param sensor The associated sensor
     *
     * @throws std::runtime_error The given AudioChannel was not known to the project.
     * @throws std::runtime_error The given Sensor was not known to the project.
     */
    void add_association(
            const Signal& signal,
            const Sensor& sensor
    );

    [[nodiscard]] bool are_signals_stored() const noexcept;

    [[nodiscard]] auto observe_signals() const noexcept
    {
        return signals | std::views::values |
               std::views::transform([](const std::unique_ptr<Signal>& container) -> const Signal& {
                   assert(container);
                   return *container.get();
               });
    }

    [[nodiscard]] bool are_sensors_stored() const noexcept;

    [[nodiscard]] auto observe_sensors() const noexcept
    {
        return sensors | std::views::values |
               std::views::transform([](const std::unique_ptr<Sensor>& container) -> const Sensor& {
                   assert(container);
                   return *container.get();
               });
    }

private:
    std::string project_name;

    std::unordered_map<Signal::id_type, std::unique_ptr<Signal>> signals;
    std::unordered_map<Sensor::id_type, std::unique_ptr<Sensor>> sensors;

    BidirectionalUnorderedMapping<Signal::id_type, Sensor::id_type> channel_mapping;
};

} // namespace WebCFD

#endif // WEBCFD_PROJECT_HPP
