/**
 * @file
 *
 * ProjectSelectorFwd specification
 *
 * @author Oliver Dixon
 * @date 2026-07-25
 */

#ifndef ECHOMAP_PROJECTSELECTORFWD_HPP
#define ECHOMAP_PROJECTSELECTORFWD_HPP

namespace echomap
{

#ifdef __EMSCRIPTEN__
class PartialProject;
using ProjectT = PartialProject;
#else
class Project;
using ProjectT = Project;
#endif // __EMSCRIPTEN__

}

#endif // ECHOMAP_PROJECTSELECTORFWD_HPP
