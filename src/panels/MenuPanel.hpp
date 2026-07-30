//
// Created by owd on 25/06/2026.
//

#ifndef ECHOMAP_MENUPANEL_HPP
#define ECHOMAP_MENUPANEL_HPP

#include <string>

#include "IProjectPanel.hpp"

namespace echomap
{

class IProjectOpenService;

class MenuPanel final : public IProjectPanel
{
public:
    explicit MenuPanel(IProjectOpenService& project_opener);

    void draw() noexcept override;

    [[nodiscard]] const char* get_imgui_name() const noexcept override;

private:
    IProjectOpenService& project_opener;

    std::string panel_name = "Menu";
};

} // namespace echomap

#endif // ECHOMAP_MENUPANEL_HPP
