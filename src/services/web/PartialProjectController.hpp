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
#include "../ProjectControllerBase.hpp"

namespace echomap
{

class INotificationSink;
class PartialProject;
class Worker;
class VFSPicker;

struct CompleteProjectLoadNotification;

/**
 * @todo Document
 */
class PartialProjectController : public ProjectControllerBase
{
public:
    explicit PartialProjectController(
            PanelHost& panel_host,
            INotificationSink& notification_sink,
            Worker& worker
    );

    ~PartialProjectController() noexcept override;

    PartialProjectController(const PartialProjectController&) = delete;
    PartialProjectController& operator=(const PartialProjectController&) = delete;

    PartialProjectController(PartialProjectController&&) = delete;
    PartialProjectController& operator=(PartialProjectController&&) = delete;

    void request_vfs_mapping(
            id_type intended_project_id,
            const std::filesystem::path& intended_external
    ) const;

    void cancel_project_load(id_type intended_project_id);
    void complete_project_load(id_type intended_project_id);

    using ProjectControllerBase::handle_notification;

    void handle_notification(const CompleteProjectLoadNotification& notification) const;

    void handle_result(LoadProjectResult&& result) override;
    void handle_result(LoadSignalFileResult&& result) override;

private:
    std::unique_ptr<PartialProject> partial_project;
    std::unique_ptr<VFSPicker> vfs_picker;
    Worker& worker;
    INotificationSink& notification_sink;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_PARTIALPROJECTCONTROLLER_HPP
