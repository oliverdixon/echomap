/**
 * @file
 *
 * IndividualUploadModal implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-17
 */

#include "IndividualUploadModal.hpp"

#include "../Logger.hpp"
#include "../actions/ActionController.hpp"
#include "../objects/Project.hpp"

namespace echomap
{

IndividualUploadModal::IndividualUploadModal(
        const Project* const project
) :
    project(project)
{
}

void IndividualUploadModal::draw() noexcept
{
    ImGui::OpenPopup("Upload External Files##UploadExternalModal");
    if (ImGui::BeginPopupModal(
                "Upload External Files##UploadExternalModal",
                nullptr,
                ImGuiWindowFlags_AlwaysAutoResize
        )) {
        ImGui::PushID("UploadExternalModal");

        ImGui::TextWrapped(
                "%s contains references to externally sources signals. Browser security requires that each externally "
                "sourced file is uploaded separately.",
                project->get_imgui_name()
        );

        ImGui::Spacing();
        ImGui::Separator();

        ImGui::TextWrapped(
                "Upload all missing files to complete the mapping, and press Continue.\n\nCancelling the operation "
                "will abort the load and revert back to the previously loaded project, if applicable."
        );

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::BeginTable("##UploadTable", 5, table_flags)) {
            ImGui::TableSetupColumn("##UploadButton", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Signal Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Indicated Path", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Indicated Channel", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Given Path", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();

            std::size_t row_entry = 0;
            for (const auto& factory : project->unloaded_signals | std::views::values) {
                if (factory == nullptr || !factory->observe_signal().observe_source().has_value()) {
                    LOG_F_WARN("Received an empty factory on row {}. Is the project corrupted?", row_entry);
                    continue;
                }

                const auto& partial_signal = factory->observe_signal();

                ImGui::PushID(static_cast<int>(row_entry++));
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                if (ImGui::Button("Upload"))
                    ActionController::complete_signal_load(project->get_id(), partial_signal.get_id());

                ImGui::TableNextColumn();

                ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
                ImGui::TextUnformatted(partial_signal.get_imgui_name());
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(partial_signal.observe_source()->path.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%ld", partial_signal.observe_source()->channel);
                ImGui::TableNextColumn();

                const auto& given_path = partial_signal.observe_source()->real_path;
                ImGui::TextUnformatted(given_path.has_value() ? given_path->c_str() : "Not provided");

                ImGui::PopID();
            }

            ImGui::EndTable();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        constexpr float button_width = 80.0f;

        if (ImGui::Button("Cancel", ImVec2(button_width, 0.0f)))
            ImGui::CloseCurrentPopup();

        ImGui::SameLine();

        if (ImGui::Button("Continue", ImVec2(button_width, 0.0f)))
            ImGui::CloseCurrentPopup();

        ImGui::PopID();
        ImGui::EndPopup();
    }
}

const char* IndividualUploadModal::get_imgui_name() const noexcept
{
    return panel_name.c_str();
}

} // namespace echomap
