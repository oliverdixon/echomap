//
// Created by owd on 25/06/2026.
//

#ifndef ECHOMAP_MENUPANEL_HPP
#define ECHOMAP_MENUPANEL_HPP

#include <string>

#include "IPanel.hpp"

namespace echomap
{

class IProjectOpenService;

class MenuPanel final : public IPanel
{
public:
    /**
     * Creates a MenuPanel to display an application-wide top menu.
     *
     * @param project_opener Service for prompting to open a new Project.
     */
    explicit MenuPanel(IProjectOpenService& project_opener);

    MenuPanel(const MenuPanel&) = delete;
    MenuPanel& operator=(const MenuPanel&) = delete;
    MenuPanel(MenuPanel&&) = delete;
    MenuPanel& operator=(MenuPanel&&) = delete;

    ~MenuPanel() override = default;

    void draw() noexcept override;

    [[nodiscard]] const char* get_imgui_name() const noexcept override;

private:
    IProjectOpenService& project_opener;

    std::string panel_name = "Menu";
};

} // namespace echomap

#endif // ECHOMAP_MENUPANEL_HPP
