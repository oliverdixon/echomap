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

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include <filesystem>

#include "../../objects/IDAllocator.hpp"
#include "../../services/web/IPartialProjectBuilderService.hpp"
#include "../../services/web/IPartialProjectCompletionService.hpp"
#include "../../services/web/IPartialProjectObserveService.hpp"
#include "../ProjectControllerBase.hpp"

namespace echomap
{

class VFSPickerService;
class PartialProject;
class Worker;

/**
 * @todo Document
 */
class PartialProjectController : public ProjectControllerBase,
                                 public IPartialProjectObserveService,
                                 public IPartialProjectCompletionService,
                                 public IPartialProjectBuilderService
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

    void add_vfs_mapping(
            id_type intended_project_id,
            const std::filesystem::path& intended_external
    ) override;

    void cancel_project_load(id_type intended_project_id) override;
    void complete_project_load(id_type intended_project_id) override;

    void handle_result(LoadProjectResult&& result) override;
    void handle_result(LoadSignalFileResult&& result) override;

private:
    void handle_complete_mapping(
            id_type project_id,
            const std::filesystem::path& external,
            std::filesystem::path internal
    ) const;

    static void handle_cancelled_mapping(const std::filesystem::path& external);

    std::unique_ptr<PartialProject> partial_project;
    std::unique_ptr<VFSPickerService> vfs_picker;
    Worker& worker;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_PARTIALPROJECTCONTROLLER_HPP
