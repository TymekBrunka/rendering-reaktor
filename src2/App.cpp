#include <App.hpp>
#include <imgui.h>
#include <rlImGui.h>

#include "embeded/cubemap.png.cpp"
#include "embeded/icon.png.cpp"
#include "embeded/icons.png.cpp"

void App::initialise() {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(800, 600, "reaktory");

  Image icon_ = {
      .data = icon_data.data,
      .width = icon_data.width,
      .height = icon_data.height,
      .mipmaps = 1,
      .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };

  Image icons_ = {
      .data = iconsImg.data,
      .width = iconsImg.width,
      .height = iconsImg.height,
      .mipmaps = 1,
      .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };

  Image skybox_ = {
      .data = cubemapImg.data,
      .width = cubemapImg.width,
      .height = cubemapImg.height,
      .mipmaps = 2,
      .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };

  SetWindowIcon(icon_);

  assets.icon = LoadTextureFromImage(icon_);
  assets.icons = LoadTextureFromImage(icons_);
  assets.skybox = LoadTextureFromImage(skybox_);

  rlImGuiSetup(true);
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  BlendMode(BLEND_ALPHA);
}

void App::run() {
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLANK);

    draw_scene();

    rlImGuiBegin();
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(0x2f151515)); // workaround to make docked windows transparent
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    panel_ui();

    ImGui::PopStyleColor(1);

    ImGui::ShowDemoWindow();
    rlImGuiEnd();

    EndDrawing();
  }
}

void App::panel_ui() {
  ImGui::SetNextWindowBgAlpha(0.5f);
  if (ImGui::Begin("ThePanel")) {
    rlImGuiImageSize(&assets.icon, 20, 20);
    ImGui::SameLine();
    ImGui::Text("reaktory");
  }
  ImGui::End();
}

void App::render_scene() {
  DrawText("Reaktory", 20, 20, 20, BLUE);
  DrawTexture(assets.icon, 30, 30, WHITE);
}

void App::cleanup() {
  rlImGuiShutdown();
  CloseWindow();
}
