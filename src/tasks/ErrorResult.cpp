//
// Created by owd on 7/9/26.
//

#include "ErrorResult.hpp"

namespace echomap
{

ErrorResult::ErrorResult(
        const std::string_view message,
        const std::source_location location,
        std::unique_ptr<ITask> responsible_task
) :
    message(message),
    location(location),
    responsible_task(std::move(responsible_task))
{
}

std::string_view ErrorResult::what() const noexcept
{
    return message;
}

const std::source_location& ErrorResult::where() const noexcept
{
    return location;
}

const ITask* ErrorResult::observe_responsible_task() const noexcept
{
    return responsible_task.get();
}

} // namespace echomap
