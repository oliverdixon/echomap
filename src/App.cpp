/**
 * @file
 * @brief EchoMap entry point
 * @author Oliver Dixon
 * @date 2026-05-05
 */

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)
#include "platform/EchoMapWeb.hpp"
#else
#include "platform/EchoMapNative.hpp"
#endif // __EMSCRIPTEN__

#include "errors/ConfigurationError.hpp"
#include "utility/Logger.hpp"

/**
 * EchoMap common entry point.
 *
 * @return OS status exit code. 0 for success, 1 for failure.
 */
int main()
{
    try {
#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

        // NOLINTBEGIN(*-owning-memory, *-cplusplus.NewDeleteLeaks) - Emscripten will manage our heap memory.

        auto* const application = new echomap::EchoMapWeb();
        application->run_event_loop();

        // NOLINTEND(*-owning-memory, *-cplusplus.NewDeleteLeaks)

#else

        // Native platforms can use the RAII facilities of EchoMap and StaticInstanceController.

        echomap::EchoMapNative application;
        application.run_event_loop();

#endif
    } catch (const echomap::ConfigurationError& error) {
        echomap::Logger::log(echomap::Logger::Level::Error, error.what(), error.where());
        return 1;
    }

    return 0;
}
