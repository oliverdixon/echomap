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
 * @return OS status exit code. 0 for success, 1 for typed error, 2 for unknown error.
 */
int main()
{
    using namespace echomap;

    try {
#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

        // NOLINTBEGIN(*-owning-memory, *-cplusplus.NewDeleteLeaks) - Emscripten will manage our heap memory.

        auto* const application = new EchoMapWeb();
        application->run_event_loop();

        // NOLINTEND(*-owning-memory, *-cplusplus.NewDeleteLeaks)

#else

        // Native platforms can use the RAII facilities of EchoMap.

        EchoMapNative application;
        application.run_event_loop();

#endif
    } catch (const echomap::ConfigurationError& error) {
        Logger::log(Logger::Level::Error, error.what(), error.where());
        return 1;
    } catch (...) {
        LOG_ERROR("Implementation-defined unknown error caught in entry point.");
        return 2;
    }

    return 0;
}
