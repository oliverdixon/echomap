/**
 * @file
 * @brief EchoMap ConfigurationError exception specification
 * @author Oliver Dixon
 * @date 2026-06-19
 */

#ifndef ECHOMAP_CONFIGURATIONERROR_H
#define ECHOMAP_CONFIGURATIONERROR_H

#include "LocatableError.hpp"

namespace echomap
{

/**
 * A ConfigurationError indicates an error encountered during the initial configuration of the EchoMap graphics context.
 */
class ConfigurationError : public LocatableError
{
public:
    /**
     * Create a new ConfigurationError with the given message at the given location.
     *
     * @param message Human-readable message indicating the cause of the error.
     * @param location Optional location of the throwing statement.
     */
    explicit ConfigurationError(
            std::string_view message,
            std::source_location location = std::source_location::current()
    );
};

} // namespace echomap

#endif // ECHOMAP_CONFIGURATIONERROR_H
