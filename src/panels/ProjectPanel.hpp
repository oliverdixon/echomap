//
// Created by owd on 25/06/2026.
//

#ifndef ECHOMAP_PROJECTPANEL_HPP
#define ECHOMAP_PROJECTPANEL_HPP

#include "../objects/Project.hpp"
#include "IPanel.hpp"

namespace EchoMap
{

class ProjectPanel final : public IPanel
{
public:
    explicit ProjectPanel(Project * initial_project = nullptr);

    void draw() noexcept override;

    [[nodiscard]] const char* get_imgui_name() const noexcept override;

    void set_active_project(Project * new_active_project) noexcept override;

private:
    const std::string panel_name = "Project Explorer";

    Project * active_project = nullptr;
};

} // namespace EchoMap

#endif // ECHOMAP_PROJECTPANEL_HPP
