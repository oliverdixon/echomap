/**
 * @file
 *
 * DownsampleTask implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-13
 */

#include "DownsampleTask.hpp"

#include "../../objects/FrequencySpectrum.hpp"
#include "../../objects/Project.hpp"
#include "../../objects/factories/SignalFactory.hpp"
#include "../results/DownsampleResult.hpp"

namespace echomap
{

DownsampleTask::DownsampleTask(
        std::weak_ptr<Signal> signal,
        const float factor
) :
    ITask(std::format(
            "DownsampleTask: {}",
            signal.lock()->get_name()
    )),
    signal(std::move(signal)),
    factor(factor)
{
    assert(!this->signal.expired());
}

DownsampleTask::~DownsampleTask() noexcept = default;

DownsampleTask::DownsampleTask(
        DownsampleTask&& other
) noexcept :
    ITask(std::move(other)),
    signal(std::move(other.signal)),
    factor(other.factor)
{
}

DownsampleTask& DownsampleTask::operator=(
        DownsampleTask&& other
) noexcept
{
    if (this == &other)
        return *this;

    move_task_identity_from(std::move(other));

    signal = std::move(other.signal);
    factor = other.factor;

    return *this;
}

WorkerResult DownsampleTask::execute_work()
{
    if (const auto owned_signal = signal.lock())
        return DownsampleResult(owned_signal->get_id(), SignalFactory::downsample(*owned_signal, factor));

    throw std::runtime_error("The Signal has expired.");
}

} // namespace echomap
