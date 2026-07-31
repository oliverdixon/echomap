//
// Created by owd on 25/06/2026.
//

#ifndef ECHOMAP_PROJECTPANEL_HPP
#define ECHOMAP_PROJECTPANEL_HPP

#include <string>

#include "IPanel.hpp"

namespace echomap
{

class IProjectObserveService;

class ProjectPanel final : public IPanel
{
public:
    /**
     * Create a new ProjectPanel to display Project metadata.
     *
     * @param observer_service Service for observing the active Project.
     */
    explicit ProjectPanel(const IProjectObserveService& observer_service);

    ProjectPanel(const ProjectPanel&) = delete;
    ProjectPanel& operator=(const ProjectPanel&) = delete;
    ProjectPanel(ProjectPanel&&) = delete;
    ProjectPanel& operator=(ProjectPanel&&) = delete;

    ~ProjectPanel() override = default;

    void draw() override;

    [[nodiscard]] const char* get_imgui_name() const noexcept override;

    static const char* get_imgui_stable_name() noexcept;

private:
    std::string panel_name;
    const IProjectObserveService& observer_service;
};

} // namespace echomap

#endif // ECHOMAP_PROJECTPANEL_HPP
