//
// Created by owd on 05/07/2026.
//

#include "LocatableError.hpp"

namespace WebCFD
{

LocatableError::LocatableError(
        const std::string_view message,
        const std::source_location location
) :
    std::runtime_error(std::string(message)),
    location(location)
{
}

const std::source_location& LocatableError::where() const noexcept
{
    return location;
}

} // namespace WebCFD
