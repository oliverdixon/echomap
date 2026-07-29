//
// Created by owd on 25/06/2026.
//

#ifndef ECHOMAP_MENUPANEL_HPP
#define ECHOMAP_MENUPANEL_HPP

#include <string>

#include "IProjectPanel.hpp"

namespace echomap
{

class ProjectControllerBase;

class MenuPanel final : public IProjectPanel
{
public:
    explicit MenuPanel(ProjectControllerBase& project_controller);

    void draw() noexcept override;

    [[nodiscard]] const char* get_imgui_name() const noexcept override;

    void change_active_project(const Project* new_project) override;

private:
    ProjectControllerBase& project_controller;

    std::string panel_name = "Menu";
};

} // namespace echomap

#endif // ECHOMAP_MENUPANEL_HPP
