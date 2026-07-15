//
// Created by owd on 7/9/26.
//

#ifndef ECHOMAP_ERRORRESULT_HPP
#define ECHOMAP_ERRORRESULT_HPP

#include <memory>
#include <source_location>
#include <string>
#include <string_view>

#include "../tasks/ITask.hpp"

namespace echomap
{

class ErrorResult
{
public:
    explicit ErrorResult(
            std::string_view message,
            std::source_location location,
            std::unique_ptr<ITask> responsible_task = nullptr
    );

    [[nodiscard]] std::string_view what() const noexcept;
    [[nodiscard]] const std::source_location& where() const noexcept;
    [[nodiscard]] const ITask * observe_responsible_task() const noexcept;

private:
    std::string message;
    std::source_location location;

    std::unique_ptr<ITask> responsible_task;
};

} // namespace echomap

#endif // ECHOMAP_ERRORRESULT_HPP
