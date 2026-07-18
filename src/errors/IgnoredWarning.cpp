/**
 * @file
 *
 * IgnoredWarning implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-19
 */

#include "IgnoredWarning.hpp"

namespace echomap
{

IgnoredWarning::IgnoredWarning(
        const std::string_view message,
        const std::source_location location
) :
    LocatableError(
            message,
            location
    )
{
}

} // namespace echomap
