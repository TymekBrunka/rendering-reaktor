#include "EditorActionsPanel.hpp"

#include "MeshManager.hpp"
#include "WorkerThreads.hpp"
#include "imgui.h"
#include <iostream>
// #include "portable-file-dialogs.h"

namespace EditorActionsPanel {

void empty() {};
void empty2(worker_status status) { std::cout << "hello mf\n"; };

void load_from_file() {
  pfd::open_file f = pfd::open_file("Wybierz plik z modelem 3D", pfd::path::home(), {"Modele 3D (.obj)", "*.obj", "Wszystkie pliki", "*"}, pfd::opt::multiselect);
  for (auto const &name : f.result())
    MeshManager::load_from_file(name);
}

void load_from_file_threaded() { workers->execute(load_from_file, MeshManager::render_thread_post_work); }

// clang-format off
actionEntry entries[] = {
    {"dodaj", ImVec2(0, 1.0), ImVec2(0.25, 0.75), empty},
    {"usun", ImVec2(0.25, 1.0), ImVec2(0.5, 0.75), empty},
    {"model", ImVec2(0.25, 0.75), ImVec2(0.5, 0.5), load_from_file_threaded},
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
      // ImGui::TaleHeadersRow();
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
    float iconSize = 35 + (widthR < heightR ? widthR : heightR);
    int cols = (int)((ImGui::GetWindowSize().x - 10) / (iconSize + 15));
    if (cols == 0)
      cols = 1;
    if (cols > amountOfEnties)
      cols = amountOfEnties;
    if (ImGui::BeginTable("EditorActionEntries", cols)) {
      for (int i = 0; i < cols; i++) {
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
      }
      actionEntry *entry = &EditorActionsPanel::entries[0];
      for (int i = 0; i < amountOfEnties; i++) {
        if (i % cols == 0)
          ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(i % cols);
        ImGui::BeginGroup();
        // ImGui::Image((ImTextureID)icons.id, ImVec2(50, 50), ImVec2(0, 1.0), ImVec2(0.25, 0.75));

        if (ImGui::ImageButton(entry->label, (ImTextureID)icons.id, ImVec2(iconSize, iconSize), entry->begin, entry->end)) {
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
