#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

int main() {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(800, 600, "reaktory");

  rlImGuiSetup(true);
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // Image icon = LoadImage("src2/icon.png");
  Texture2D icontex = LoadTexture("src2/icon.png");
  Image icon = LoadImageFromTexture(icontex);
  SetWindowIcon(icon);
  BlendMode(BLEND_ALPHA);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLANK);

    DrawText("Reaktory", 20, 20, 20, BLUE);
    DrawTexture(icontex, 30, 30, WHITE);

    rlImGuiBegin();

    ImGui::ShowDemoWindow();
    // ImGui::SetWindowPos(ImVec2(10, 10));
    // ImGui::Begin("overlay");
    // ImGui::End();

    rlImGuiEnd();

    EndDrawing();
  }
  rlImGuiShutdown();
  CloseWindow();
}
