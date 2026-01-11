#include "EditorActionsPanel.hpp"

#include "MeshManager.hpp"
namespace EditorActionsPanel {

void empty() {};

// clang-format off
actionEntry entries[] = {
    {"dodaj", ImVec2(0, 1.0), ImVec2(0.25, 0.75), MeshManager::load_from_file},
    {"usun", ImVec2(0.25, 1.0), ImVec2(0.5, 0.75), empty},
    {"model", ImVec2(0.25, 0.75), ImVec2(0.5, 0.5), empty},
    {"zdjecie", ImVec2(0.5, 0.75), ImVec2(0.75, 0.5), empty},
    {"zaladuj", ImVec2(0.75, 0.75), ImVec2(1.0, 0.5), empty},
    {"zrzut", ImVec2(0, 0.5), ImVec2(0.25, 0.25), empty},
};
// clang-format on

constexpr int amountOfEnties = sizeof(EditorActionsPanel::entries) / sizeof(EditorActionsPanel::actionEntry);

void UI(RR::Texture2d &icon, RR::Texture2d &icons) {
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 30);
  ImGui::Begin("Panel", NULL);
  {
    if (ImGui::BeginTable("hIcon", 2)) {
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

    float widthR = (ImGui::GetWindowSize().x * 0.05);
    float heightR = ((ImGui::GetWindowSize().y - 100) * 0.25);
    float iconSize = 40 + (widthR < heightR ? widthR : heightR);
    int cols = (int)((ImGui::GetWindowSize().x - 10) / (iconSize + 15));
    if (cols == 0)
      cols = 1;
    if (cols > amountOfEnties)
      cols = amountOfEnties;

    if (ImGui::BeginTable("hIcon", cols)) {
      for (int i = 0; i < cols; i++) {
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
      }
      actionEntry *entry = &EditorActionsPanel::entries[0];
      for (int i = 0; i < amountOfEnties; i++) {
        if (i % cols == 0)
          ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(i % cols);
        ImGui::BeginGroup();
        // ImGui::Image((ImTextureID)icons.id, ImVec2(50, 50), ImVec2(0, 1.0), ImVec2(0.25, 0.75));

        if (ImGui::ImageButton(entry->label, (ImTextureID)icons.id, ImVec2(50, 50), entry->begin, entry->end)) {
          entry->callback();
        }
        ImGui::Text(entry->label);

        ImGui::EndGroup();
        entry++;
      }
      ImGui::EndTable();
    }

    ImGui::End();
    ImGui::PopStyleVar(1);
  }
}
} // namespace EditorActionsPanel
