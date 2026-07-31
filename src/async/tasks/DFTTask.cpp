/**
 * @file
 *
 * DFTTask implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-14
 */

#include "DFTTask.hpp"

#include "../../objects/Signal.hpp"
#include "../../objects/factories/FrequencySpectrumFactory.hpp"
#include "../results/DFTResult.hpp"

namespace echomap
{

DFTTask::DFTTask(
        std::weak_ptr<Signal> signal,
        const WindowFunctions::AllFunctions window_function,
        const std::size_t transform_size
) :
    ITask(std::format(
            "DFTTask: {}",
            signal.lock()->get_name()
    )),
    signal(std::move(signal)),
    window_function(window_function),
    transform_size(transform_size)
{
    assert(!this->signal.expired());
}

DFTTask::~DFTTask() noexcept = default;

DFTTask::DFTTask(
        DFTTask&& other
) noexcept :
    ITask(std::move(other)),
    signal(std::move(other.signal)),
    window_function(other.window_function),
    transform_size(other.transform_size)
{
}

DFTTask& DFTTask::operator=(
        DFTTask&& other
) noexcept
{
    if (this == &other)
        return *this;

    move_task_identity_from(std::move(other));

    signal = std::move(other.signal);
    window_function = other.window_function;
    transform_size = other.transform_size;

    return *this;
}

WorkerResult DFTTask::execute_work()
{
    if (const auto owned_signal = signal.lock())
        return DFTResult(
                owned_signal->get_id(),
                FrequencySpectrumFactory::create_frequency_spectrum(*owned_signal, window_function, transform_size),
                transform_size
        );

    throw std::runtime_error("The Signal has expired.");
}

} // namespace echomap
