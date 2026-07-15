//
// Created by owd on 25/06/2026.
//

#ifndef ECHOMAP_PROJECTPANEL_HPP
#define ECHOMAP_PROJECTPANEL_HPP

#include "../objects/Project.hpp"
#include "../tasks/ScopedConnections.hpp"
#include "IPanel.hpp"

namespace echomap
{

class WorkerResultDespatcher;

class ProjectPanel final : public IPanel
{
public:
    explicit ProjectPanel(
            WorkerResultDespatcher& despatcher,
            const Project* initial_project = nullptr
    );

    void draw() noexcept override;

    [[nodiscard]] const char* get_imgui_name() const noexcept override;

private:
    const std::string panel_name = "Project Explorer";

    const Project* active_project = nullptr;
    ScopedConnections connections;
};

} // namespace echomap

#endif // ECHOMAP_PROJECTPANEL_HPP
