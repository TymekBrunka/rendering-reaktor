#include "EditorActionsPanel.hpp"
namespace EditorActionsPanel {

	actionEntry entries[] = {
		{"dodaj", ImVec2(0, 1.0), ImVec2(0.25, 0.75)},
		{"usun", ImVec2(0.25, 1.0), ImVec2(0.5, 0.75)},
		{"model", ImVec2(0.25, 0.75), ImVec2(0.5, 0.5)},
		{"zdjecie", ImVec2(0.5, 0.75), ImVec2(0.75, 0.5)},
		{"zaladuj", ImVec2(0.75, 0.75), ImVec2(1.0, 0.5)},
		{"zrzut", ImVec2(0, 0.5), ImVec2(0.25, 0.25)},
	};

	constexpr int amountOfEnties = sizeof(EditorActionsPanel::entries) / sizeof(EditorActionsPanel::actionEntry);

	void UI(RR::Texture2d& icon, RR::Texture2d& icons) {
		ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 30);
		ImGui::Begin("Panel", NULL);
		{
			if (ImGui::BeginTable("hIcon", 2))
			{
				ImGui::TableSetupColumn("icon", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("name", ImGuiTableColumnFlags_WidthFixed);
				// ImGui::TableHeadersRow();

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Image((ImTextureID)icon.id, ImVec2(30, 30));
				// ImGui::SetCursorPos(ImVec2(40, 32));
				// ImGui::TableNextColumn();
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("Reaktory");

				ImGui::EndTable();
			}
			// ImGui::
			float widthR = (ImGui::GetWindowSize().x * 0.05);
			float heightR = ((ImGui::GetWindowSize().y - 100) * 0.25);
			float iconSize = 40 + (widthR < heightR ? widthR : heightR);
			int cols = (int)((ImGui::GetWindowSize().x - 10) / (iconSize + 15));
			if (cols == 0) cols = 1;
			if (cols > amountOfEnties) cols = amountOfEnties;
			if (ImGui::BeginTable("actionIcons", cols))
			{
				for (int i = 0; i < cols; i++) {
					ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
				}
				for (int i = 0; i < amountOfEnties; i++) {
					if (i % cols == 0) ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(i % cols);
					ImGui::BeginGroup();
						// ImGui::Image((ImTextureID)icons.id, ImVec2(50, 50), ImVec2(0, 1.0), ImVec2(0.25, 0.75));
						ImGui::ImageButton(
								EditorActionsPanel::entries[i].label,
								(ImTextureID)icons.id, ImVec2(iconSize, iconSize),
								EditorActionsPanel::entries[i].begin,
								EditorActionsPanel::entries[i].end);
						ImGui::Text(EditorActionsPanel::entries[i].label);
					ImGui::EndGroup();
				}
				ImGui::EndTable();
			}

			ImGui::End();
			ImGui::PopStyleVar(1);
		}
	}
}
