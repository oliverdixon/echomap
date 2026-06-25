/**
 * @file
 * @brief WebCFD main viewport panel specification
 * @author Oliver Dixon
 * @date 2026-06-20
 */

#ifndef WEBCFD_VIEWPORTPANEL_HPP
#define WEBCFD_VIEWPORTPANEL_HPP

#include <implot.h>

#include <string>

#include "../Project.hpp"
#include "IPanel.hpp"

namespace WebCFD
{

class ViewportPanel final : public IPanel
{
public:
    ViewportPanel();

    explicit ViewportPanel(Project * project);

    [[nodiscard]] const char* get_imgui_name() const noexcept override;

    void draw() noexcept override;

private:
    const std::string panel_name = "Manager";
    ImPlotSpec plotting_spec;
    Project * active_project = nullptr;
};

} // namespace WebCFD

#endif // WEBCFD_VIEWPORTPANEL_HPP
