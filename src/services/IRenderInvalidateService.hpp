/**
 * @file
 *
 * IRenderInvalidateService specification
 *
 * @author Oliver Dixon
 * @date 2026-07-28
 */

#ifndef ECHOMAP_IRENDERINVALIDATESERVICE_HPP
#define ECHOMAP_IRENDERINVALIDATESERVICE_HPP

namespace echomap
{

/**
 * Service interface for forcing a renderer to redraw a fixed number of frames.
 */
class IRenderInvalidateService
{
public:
    virtual ~IRenderInvalidateService() = default;

    IRenderInvalidateService(const IRenderInvalidateService&) = default;
    IRenderInvalidateService& operator=(const IRenderInvalidateService&) = default;
    IRenderInvalidateService(IRenderInvalidateService&&) = default;
    IRenderInvalidateService& operator=(IRenderInvalidateService&&) = default;

    /**
     * Indicate to the renderer that the following frames should always be rendered, regardless of whether there are any
     * new events to process.
     *
     * The forced frame count does not accumulate; callers may request forced frames independently. By default, we force
     * four frames since most Dear ImGui components can fully render a four-frame cycle.
     *
     * @param count The number of frames to force.
     */
    virtual void force_frames(unsigned int count = 4) noexcept = 0;

protected:
    IRenderInvalidateService() = default;
};

} // namespace echomap

#endif // ECHOMAP_IRENDERINVALIDATESERVICE_HPP
