/**
 * @file
 *
 * FullProjectController specification
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#ifndef ECHOMAP_FULLPROJECTCONTROLLER_HPP
#define ECHOMAP_FULLPROJECTCONTROLLER_HPP

#if !defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "../ProjectControllerBase.hpp"

namespace echomap
{

/**
 * @todo Document
 */
class FullProjectController : public ProjectControllerBase<FullProjectController>
{
public:
    explicit FullProjectController(PanelHost& panel_host);

    ~FullProjectController() noexcept;

    FullProjectController(const FullProjectController&) = delete;
    FullProjectController& operator=(const FullProjectController&) = delete;

    FullProjectController(FullProjectController&&) = delete;
    FullProjectController& operator=(FullProjectController&&) = delete;

private:
    friend ProjectControllerBase;

    void handle_result_impl(LoadProjectResult&& result);
    void handle_result_impl(LoadSignalFileResult&& result) const;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_FULLPROJECTCONTROLLER_HPP
