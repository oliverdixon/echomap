/**
 * @file
 *
 * IWebPanel specification
 *
 * @author Oliver Dixon
 * @date 2026-07-25
 */

#ifndef ECHOMAP_IWEBPANEL_HPP
#define ECHOMAP_IWEBPANEL_HPP

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "../IPanel.hpp"

namespace echomap
{

class PartialProject;

/**
 * An IPanel which can receive updates to the application-wide unloaded PartialProject.
 */
class IWebPanel : public IPanel
{
public:
    /**
     * Updates the active Project being described by the IPanel.
     *
     * @param new_project An observing pointer to the new active Project.
     */
    virtual void change_active_project(const PartialProject * new_project) = 0;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_IWEBPANEL_HPP
