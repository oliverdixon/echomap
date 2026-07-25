/**
 * @file
 *
 * IProjectPanel specification
 *
 * @author Oliver Dixon
 * @date 2026-07-25
 */

#ifndef ECHOMAP_IPROJECTPANEL_HPP
#define ECHOMAP_IPROJECTPANEL_HPP

#include "IPanel.hpp"

namespace echomap
{

class Project;

/**
 * @todo Document
 */
class IProjectPanel : public IPanel
{
public:
    /**
     * Updates the active Project being described by the IPanel.
     *
     * @param new_project An observing pointer to the new active Project.
     */
    virtual void change_active_project(const Project * new_project) = 0;
};

} // namespace echomap

#endif // ECHOMAP_IPROJECTPANEL_HPP
