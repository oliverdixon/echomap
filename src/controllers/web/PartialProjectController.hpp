/**
 * @file
 *
 * PartialProjectController specification
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#ifndef ECHOMAP_PARTIALPROJECTCONTROLLER_HPP
#define ECHOMAP_PARTIALPROJECTCONTROLLER_HPP
#include <__filesystem/path.h>

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "../../objects/IDAllocator.hpp"
#include "../../services/web/IPartialProjectCompletionService.hpp"
#include "../../services/web/IPartialProjectObserveService.hpp"
#include "../../services/web/IVFSRequestService.hpp"
#include "../ProjectControllerBase.hpp"

namespace echomap
{

class PartialProject;
class Worker;
class VFSPicker;

/**
 * @todo Document
 */
class PartialProjectController : public ProjectControllerBase,
                                 public IPartialProjectObserveService,
                                 public IPartialProjectCompletionService,
                                 public IVFSRequestService // TODO don't implement this.
{
public:
    explicit PartialProjectController(
            PanelHost& panel_host,
            Worker& worker
    );

    ~PartialProjectController() noexcept override;

    PartialProjectController(const PartialProjectController&) = delete;
    PartialProjectController& operator=(const PartialProjectController&) = delete;

    PartialProjectController(PartialProjectController&&) = delete;
    PartialProjectController& operator=(PartialProjectController&&) = delete;

    [[nodiscard]] const PartialProject* observe_partial_project() const noexcept override;

    void request_vfs_mapping(
            id_type intended_project_id,
            const std::filesystem::path& intended_external
    ) override;

    void cancel_project_load(id_type intended_project_id) override;
    void complete_project_load(id_type intended_project_id) override;

    void handle_result(LoadProjectResult&& result) override;
    void handle_result(LoadSignalFileResult&& result) override;

private:
    std::unique_ptr<PartialProject> partial_project;
    std::unique_ptr<VFSPicker> vfs_picker;
    Worker& worker;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_PARTIALPROJECTCONTROLLER_HPP
