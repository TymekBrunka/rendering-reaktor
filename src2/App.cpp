#include "AssetMgr/ModelMgr.hpp"
#include "SDL3/SDL_dialog.h"
#include "raylib.h"
#include <App.hpp>
#include <SDL3/SDL.h>
#include <imgui.h>
#include <iostream>
#include <rlImGui.h>
#include <rlgl.h>

#include <FPSControler.cpp>

// #include <GLFW/glfw3.h>
#include <embeded/IconsFontAwesome6.h>
#include <embeded/RobotoRegular.h>
#include <embeded/cubemap.fs.hpp>
#include <embeded/cubemap.vs.hpp>
#include <embeded/icon.png.hpp>
#include <embeded/icons.png.hpp>
#include <embeded/skybox.frag.glsl.hpp>
#include <embeded/skybox.png.hpp>
#include <embeded/skybox.vertex.glsl.hpp>

static ImFont *font1;

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
      .data = skyboxImg.data,
      .width = skyboxImg.width,
      .height = skyboxImg.height,
      .mipmaps = 1,
      .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };

  SetWindowIcon(icon_);

  assets.icon = LoadTextureFromImage(icon_);
  assets.icons = LoadTextureFromImage(icons_);

  Mesh cube = GenMeshCube(1, 1, 1);
  skybox = LoadModelFromMesh(cube);
  skybox.materialCount = 1;

  std::cout << "Loading skybox shader\n";

  Shader skybox_shader = LoadShaderFromMemory(skybox_vertex_text, skybox_fragment_text);
  skybox.materials[0].shader = skybox_shader;

  int envmap_map[1] = {MATERIAL_MAP_CUBEMAP};
  SetShaderValue(skybox_shader, GetShaderLocation(skybox_shader, "environmentMap"), &envmap_map, SHADER_UNIFORM_INT);

  skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(skybox_, CUBEMAP_LAYOUT_AUTO_DETECT);
  SetTextureFilter(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture, TEXTURE_FILTER_POINT); // pixelated instead of blurry

  rlImGuiSetup(true);
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImFontConfig fontcfg;
  // fontcfg.PixelSnapH = true;
  // fontcfg.FontDataOwnedByAtlas = false;
  fontcfg.MergeMode = true;

  static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
  font1 = io.Fonts->AddFontFromMemoryCompressedTTF(RobotoRegular_compressed_data, RobotoRegular_compressed_size, 16.0f);
  io.Fonts->AddFontFromMemoryCompressedTTF(FA_compressed_data, FA_compressed_size, 16.0f, &fontcfg, icons_ranges);

  // BlendMode(BLEND_ALPHA);
  rlSetBlendMode(BLEND_ALPHA);

  camera.fovy = 90.0f;
  camera.projection = CAMERA_PERSPECTIVE;
  camera.position = Vector3{
      body.position.x,
      body.position.y + 1,
      body.position.z,
  };

  updateCamera();

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
}

void App::run() {
  bool mouse_locked = false;
  Vector2 last_mouse_pos = Vector2{0, 0};
  while (!WindowShouldClose()) {
    SDL_PumpEvents();
    Vector2 mouseDelta;
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
      if (!mouse_locked) {
        // SetMousePosition((float)GetRenderWidth() / 2.0f, (float)GetRenderHeight() / 2.0f);
        DisableCursor();
      } else {
        mouseDelta = GetMouseDelta();
      }
      mouse_locked = true;
    } else {
      mouseDelta = Vector2{0, 0};
      if (mouse_locked) {
        EnableCursor();
        mouse_locked = false;
      }
    }

    last_mouse_pos = GetMousePosition();

    orientation.x -= mouseDelta.x * sensitivity.x;
    orientation.y += mouseDelta.y * sensitivity.y;
    char sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
    char forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));

    updateBody();

    float delta = GetFrameTime();
    // headLerp = Lerp(headLerp, (crouching ? CROUCH_HEIGHT : STAND_HEIGHT), 20.0f * delta);
    camera.position = Vector3{
        body.position.x,
        body.position.y + 1,
        body.position.z,
    };

    walkLerp = Lerp(walkLerp, 0.0f, 10.0f * delta);
    camera.fovy = Lerp(camera.fovy, 90.0f, 5.0f * delta);

    lean.x = Lerp(lean.x, sideway * 0.02f, 10.0f * delta);
    lean.y = Lerp(lean.y, forward * 0.015f, 10.0f * delta);

    updateCamera();

    BeginDrawing();
    ClearBackground(BLANK);

    render_scene();

    rlImGuiBegin();
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(0x5f151515)); // workaround to make docked windows transparent
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_DockingSeparatorSize, 0.0f);
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::PushFont(font1);
    panel_ui();

    ImGui::PopStyleColor(1);
    ImGui::PopStyleVar(2);

    ImGui::ShowDemoWindow();
    ImGui::ShowStyleEditor();

    ImGui::PopFont();
    rlImGuiEnd();

    DrawFPS(GetRenderWidth() - MeasureText("60 FPS", 20) - 10, 10);
    EndDrawing();
  }
}

#define ICONS_MODULO 4
#define ICONS_IDX_HEIGHT 4

bool App::IconButton(const char *label, int idx, ImVec2 size) {
  ImGui::BeginGroup();
  ImVec2 start = ImVec2((idx % ICONS_MODULO) * (1.0f / ICONS_MODULO), 1.0f - ((idx / ICONS_MODULO) * (1.0f / ICONS_IDX_HEIGHT)));
  ImVec2 end = ImVec2(((idx % ICONS_MODULO) + 1) * (1.0f / ICONS_MODULO), 1.0f - (((idx / ICONS_MODULO) + 1) * (1.0f / ICONS_IDX_HEIGHT)));
  bool ret = ImGui::ImageButton("##", (ImTextureID)assets.icons.id, size, start, end);
  float txt_width = ImGui::CalcTextSize(label).x;
  if (txt_width < size.x - 5) {
    ImGui::Dummy(ImVec2(((size.x - 5) - txt_width) / 2.0, 0));
    ImGui::SameLine();
  }
  ImGui::TextUnformatted(label);
  ImGui::EndGroup();
  return ret;
}

static void SDLCALL load_model_callback(void *userdata, const char *const *filelist, int filter) {
  if (!filelist) {
    return;
  } else if (!*filelist) {
    return;
  }

  ModelMgr* model_mgr = (ModelMgr*)userdata;
  while (*filelist) {
    model_mgr->load_model(std::string{*filelist});
    filelist++;
  }
}

void App::panel_ui() {
  if (ImGui::Begin("ThePanel")) {
    rlImGuiImageSize(&assets.icon, 20, 20);
    ImGui::SameLine();
    ImGui::Text("reaktory");

    ImGui::PushID(0);
    if (IconButton("model", 5)) {
      static const SDL_DialogFileFilter ofd_filters[] = {{"Modele obj (.obj)", "obj"}, {"Wszystkie pliki", "*"}};
      SDL_ShowOpenFileDialog(load_model_callback, &this->model_mgr, nullptr, ofd_filters, 1, NULL, true);
    }
    ImGui::PopID();
    ImGui::SameLine();
    ImGui::PushID(1);
    IconButton("arkusz", 7);
    ImGui::PopID();
  }
  ImGui::End();

  if (ImGui::Begin("Modele")) {
    for (const auto& [name, model] : model_mgr.models) {
      ImGui::Button(name.c_str());
    }
    ImGui::End();
  }
}

void App::render_scene() {
  BeginMode3D(camera);
  rlDisableBackfaceCulling();
  rlDisableDepthMask();
  DrawModel(skybox, camera.position, 1.0f, WHITE);
  rlEnableBackfaceCulling();
  rlEnableDepthMask();
  EndMode3D();

  BeginMode3D(camera);
  DrawCube({-10, -15, -20}, 20, 30, 40, RED);
  EndMode3D();
}

void App::cleanup() {
  rlImGuiShutdown();
  CloseWindow();
}
