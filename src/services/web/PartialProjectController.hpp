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

#include "../ProjectControllerBase.hpp"

namespace echomap
{

class INotificationSink;
class PartialProject;
class Worker;

struct RegisterVFSMappingNotification;
struct CompleteProjectLoadNotification;
struct CancelProjectLoadNotification;

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

    ~PartialProjectController() override;

    PartialProjectController(const PartialProjectController&) = delete;
    PartialProjectController& operator=(const PartialProjectController&) = delete;

    PartialProjectController(PartialProjectController&&) = delete;
    PartialProjectController& operator=(PartialProjectController&&) = delete;

    using ProjectControllerBase::handle_notification;

    void handle_notification(const CancelProjectLoadNotification& notification);
    void handle_notification(const CompleteProjectLoadNotification& notification) const;
    void handle_notification(RegisterVFSMappingNotification& notification) const;

    void handle_result(LoadProjectResult&& result) override;
    void handle_result(LoadSignalFileResult&& result) override;

private:
    std::unique_ptr<PartialProject> partial_project;
    Worker& worker;
    INotificationSink& notification_sink;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_PARTIALPROJECTCONTROLLER_HPP
