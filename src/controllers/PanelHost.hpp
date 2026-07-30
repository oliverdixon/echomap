/**
 * @file
 *
 * PanelHost specification
 *
 * @author Oliver Dixon
 * @date 2026-07-28
 */

#ifndef ECHOMAP_PANELHOST_HPP
#define ECHOMAP_PANELHOST_HPP

#include <imgui.h>

#include <memory>
#include <vector>

namespace echomap
{
class IRenderInvalidateService;

class ErrorModal;
class IPanel;
class Project;
class WorkerResultDespatcher;
class Worker;

/**
 * Provides services for on-screen panels and modals.
 */
class PanelHost
{
public:
    PanelHost();
    ~PanelHost() noexcept;

    PanelHost(const PanelHost&) = delete;
    PanelHost& operator=(const PanelHost&) = delete;
    PanelHost(PanelHost&&) = delete;
    PanelHost& operator=(PanelHost&&) = delete;

    void add_panel(std::unique_ptr<IPanel> panel);

    void draw_all() const;
    void reset_active_modal() noexcept;
    void change_active_modal(std::unique_ptr<IPanel> modal) noexcept;

    static void configure_default_dockspace(ImGuiID dockspace_id);

    void raise_error(std::string_view message);

    void raise_error(
            std::string_view message,
            const std::runtime_error& exception
    );

    void clear_error() noexcept;

    [[nodiscard]] bool is_modal_shown() const noexcept;

private:
    std::vector<std::unique_ptr<IPanel>> panels; /**< Individual display components. */
    std::unique_ptr<IPanel> active_modal;        /**< The current active non-ErrorModal modal panel. */
    std::unique_ptr<ErrorModal> error_modal;     /**< Indicate errors over all other panels. */
};

} // namespace echomap

#endif // ECHOMAP_PANELHOST_HPP
