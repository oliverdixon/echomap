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
class FullProjectController : public ProjectControllerBase
{
public:
    explicit FullProjectController(
            std::unique_ptr<IProjectFilePicker> project_file_picker,
            PanelHost& panel_host,
            Worker& worker
    );

    ~FullProjectController() noexcept override = default;

    FullProjectController(const FullProjectController&) = delete;
    FullProjectController& operator=(const FullProjectController&) = delete;

    FullProjectController(FullProjectController&&) = delete;
    FullProjectController& operator=(FullProjectController&&) = delete;

    void handle_result(LoadProjectResult&& result) override;
    void handle_result(LoadSignalFileResult&& result) override;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_FULLPROJECTCONTROLLER_HPP
