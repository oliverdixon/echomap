/**
 * @file
 *
 * IResultHandler specification
 *
 * @author Oliver Dixon
 * @date 2026-07-11
 */

#ifndef ECHOMAP_IRESULTHANDLER_HPP
#define ECHOMAP_IRESULTHANDLER_HPP

namespace echomap
{

class LoadProjectResult;
class DownsampleResult;

/**
 * Defines an interface for handling IResult objects resulting from ITask computations.
 */
class IResultHandler
{
public:
    virtual ~IResultHandler() = default;

    virtual void handle(LoadProjectResult& result)
    {
        std::ignore = result;
    }

    virtual void handle(DownsampleResult& result)
    {
        std::ignore = result;
    }
};

} // namespace echomap

#endif // ECHOMAP_IRESULTHANDLER_HPP
