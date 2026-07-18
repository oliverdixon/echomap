/**
 * @file
 *
 * IgnoredWarning specification
 *
 * @author Oliver Dixon
 * @date 2026-07-19
 */

#ifndef ECHOMAP_IGNOREDWARNING_HPP
#define ECHOMAP_IGNOREDWARNING_HPP

#include "LocatableError.hpp"

namespace echomap
{

/**
 * Exception class to indicate a non-fatal warning indicating an exceptional case of dropping some notification.
 */
class IgnoredWarning : public LocatableError
{
public:
    /**
     * Create a new IgnoredWarning with the given message at the given location.
     *
     * @param message Human-readable message indicating the reason for the drop.
     * @param location Optional location of the throwing statement.
     */
    explicit IgnoredWarning(
            std::string_view message,
            std::source_location location = std::source_location::current()
    );
};

} // namespace echomap

#endif // ECHOMAP_IGNOREDWARNING_HPP
