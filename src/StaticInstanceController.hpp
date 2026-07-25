/**
 * @file
 *
 * StaticInstanceController specification
 *
 * @author Oliver Dixon
 * @date 2026-07-18
 */

#ifndef ECHOMAP_STATICINSTANCECONTROLLER_HPP
#define ECHOMAP_STATICINSTANCECONTROLLER_HPP

namespace echomap
{

class EchoMap;

/**
 * Provides a binding interface to connect static controllers to an instance of EchoMap.
 */
class StaticInstanceController
{
public:
    explicit StaticInstanceController(EchoMap& app);
    ~StaticInstanceController() noexcept;

    StaticInstanceController(const StaticInstanceController&) = delete;
    StaticInstanceController& operator=(const StaticInstanceController&) = delete;

    StaticInstanceController(StaticInstanceController&&) = delete;
    StaticInstanceController& operator=(StaticInstanceController&&) = delete;
};

} // namespace echomap

#endif // ECHOMAP_STATICINSTANCECONTROLLER_HPP
