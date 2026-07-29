/**
 * @file
 *
 * IRenderInvalidator specification
 *
 * @author Oliver Dixon
 * @date 2026-07-28
 */

#ifndef ECHOMAP_IRENDERINVALIDATOR_HPP
#define ECHOMAP_IRENDERINVALIDATOR_HPP

namespace echomap
{

/**
 * Service interface for forcing a renderer to redraw a fixed number of frames.
 */
class IRenderInvalidator
{
public:
    virtual ~IRenderInvalidator() = default;

    IRenderInvalidator(const IRenderInvalidator&) = default;
    IRenderInvalidator& operator=(const IRenderInvalidator&) = default;
    IRenderInvalidator(IRenderInvalidator&&) = default;
    IRenderInvalidator& operator=(IRenderInvalidator&&) = default;

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
    IRenderInvalidator() = default;
};

} // namespace echomap

#endif // ECHOMAP_IRENDERINVALIDATOR_HPP
