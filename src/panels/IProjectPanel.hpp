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
 * An IPanel which can receive updates to the application-wide active Project.
 */
class IProjectPanel : public IPanel
{
public:
};

} // namespace echomap

#endif // ECHOMAP_IPROJECTPANEL_HPP
