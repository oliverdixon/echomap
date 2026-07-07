/**
 * @file
 * @brief WebCFD ConfigurationError exception specification
 * @author Oliver Dixon
 * @date 2026-06-19
 */

#ifndef WEBCFD_CONFIGURATIONERROR_H
#define WEBCFD_CONFIGURATIONERROR_H

#include "LocatableError.hpp"

namespace WebCFD
{

/**
 * A ConfigurationError indicates an error encountered during the initial configuration of the WebCFD graphics context.
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

} // namespace WebCFD

#endif // WEBCFD_CONFIGURATIONERROR_H
