#include "AssetMgr/ModelMgr.hpp"
#include "SDL3/SDL_dialog.h"
#include "raylib.h"
#include <App.hpp>
#include <SDL3/SDL.h>
#include <cstdlib>
#include <imgui.h>
#include <iostream>
#include <rlImGui.h>
#include <rlgl.h>

#include <FPScontroler.cpp>

// #include <GLFW/glfw3.h>
#include <IconsFontAwesome6.h>
#include <RobotoRegular.h>
#include <cubemap.fs.hpp>
#include <cubemap.vs.hpp>
#include <icon.png.hpp>
#include <icons.png.hpp>
#include <skybox.frag.glsl.hpp>
#include <skybox.png.hpp>
#include <skybox.vertex.glsl.hpp>
#include <placeholder.png.hpp>

#include <skinning.vs.hpp>
#include <skinning_colorpicker.fs.hpp>

#include <Renderdoc.cpp>

std::mutex global_lock{};
std::string imported_zip_file{};

extern char *home_dir;

static ImFont *font1;

void App::initialise() {
#ifdef _WIN32
  home_dir = getenv("USERPROFILE");
#else
  home_dir = getenv("HOME");
#endif

  if (!home_dir)
    throw "Cannot find home";

  if (!LoadRenderDoc())
    std::cerr << "Couldn't load renderdoc\n";

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

  std::cerr << "Loading skybox shader\n";

  Shader skybox_shader = LoadShaderFromMemory(skybox_vertex_text, skybox_fragment_text);
  skybox.materials[0].shader = skybox_shader;

  int envmap_map[1] = {MATERIAL_MAP_CUBEMAP};
  SetShaderValue(skybox_shader, GetShaderLocation(skybox_shader, "environmentMap"), &envmap_map, SHADER_UNIFORM_INT);

  skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(skybox_, CUBEMAP_LAYOUT_AUTO_DETECT);
  SetTextureFilter(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture, TEXTURE_FILTER_POINT); // pixelated instead of blurry


  assets.colorpicker_shader = LoadShaderFromMemory(skinning_vs_text, skinning_fs_colorpicker_text);
  if (!IsShaderValid(assets.colorpicker_shader))
    std::cerr << "failed to load model(skinning+colorpicker) shader\n";
  location_id = GetShaderLocation(assets.colorpicker_shader, "ID");

  rlImGuiSetup(true);
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ImGui::GetStyle().AntiAliasedLines = false;

  ImFontConfig fontcfg;
  // fontcfg.PixelSnapH = true;
  // fontcfg.FontDataOwnedByAtlas = false;
  fontcfg.MergeMode = true;

  static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
  font1 = io.Fonts->AddFontFromMemoryCompressedTTF(RobotoRegular_compressed_data, RobotoRegular_compressed_size, 16.0f);
  io.Fonts->AddFontFromMemoryCompressedTTF(FA_compressed_data, FA_compressed_size, 16.0f, &fontcfg, icons_ranges);

  rlSetBlendMode(BLEND_ALPHA);

  camera.fovy = 90.0f;
  camera.projection = CAMERA_PERSPECTIVE;
  camera.position = Vector3{
      body.position.x,
      body.position.y + 1,
      body.position.z,
  };

  updateCamera();

  model_mgr.setup();

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
}

void App::run() {
  bool mouse_locked = false;
  Vector2 last_mouse_pos = Vector2{0, 0};
  while (!WindowShouldClose()) {
    // process file dialog actions
    SDL_PumpEvents();
    global_lock.lock();

    if (models_to_load.size() > 0) {
      for (const auto &model : models_to_load) {
        model_mgr.load_model(model);
      }
      models_to_load.clear();
    }
    if (!imported_zip_file.empty()) {
      import_scene_zip(imported_zip_file.c_str());
      imported_zip_file.clear();
    }

    global_lock.unlock();

    // scene rendering and 3d character controler
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

    if (RenderDocIsFrameCapturing())
      RenderDocBeginFrameCapture();

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

    if (RenderDocIsFrameCapturing())
      RenderDocEndFrameCapture();
  }
}

static void SDLCALL load_zip_callback(void *userdata, const char *const *filelist, int filter) {
  if (!filelist) {
    return;
  } else if (!*filelist) {
    return;
  }

  global_lock.lock();
  imported_zip_file = *filelist;
  global_lock.unlock();
}

static void SDLCALL load_model_callback(void *userdata, const char *const *filelist, int filter) {
  if (!filelist) {
    return;
  } else if (!*filelist) {
    return;
  }

  std::vector<std::string> *models_to_load = (std::vector<std::string> *)userdata;
  global_lock.lock();
  while (*filelist) {
    models_to_load->push_back(std::string{*filelist});
    filelist++;
  }
  global_lock.unlock();
}

struct PanelIcon {
  int icon;
  const char *label;
  void (*onclick)(App *);
};

PanelIcon panel_icons[] = {
    {5, "model",
     [](App *app) {
       static const SDL_DialogFileFilter ofd_filters[] = {{"Modele obj (.obj)", "obj"}, {"Wszystkie pliki", "*"}};
       SDL_ShowOpenFileDialog(load_model_callback, &app->models_to_load, nullptr, ofd_filters, 2, NULL, true);
     }},

    {7, "arkusz", [](App *app) {}},

    {9, "załaduj",
     [](App *app) {
       static const SDL_DialogFileFilter ofd_filters[] = {{"Archiwum zip (.zip)", "zip"}, {"Wszystkie pliki", "*"}};
       SDL_ShowOpenFileDialog(load_zip_callback, NULL, nullptr, ofd_filters, 2, NULL, false);
     }},
};

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

void App::panel_ui() {
  if (ImGui::Begin("ThePanel")) {
    rlImGuiImageSize(&assets.icon, 20, 20);
    ImGui::SameLine();
    ImGui::Text("reaktory");

    int i = 0;
    float width_sum = 0;
    for (const auto &icon : panel_icons) {
      ImGui::PushID(i);
      if (IconButton(icon.label, icon.icon)) {
        icon.onclick(this);
      }
      ImGui::PopID();

      if (ImGui::GetContentRegionAvail().x - width_sum > 90 - 5) { // 2 elements - 5px
        ImGui::SameLine();
        width_sum += 45;
      } else {
        width_sum = 0;
      }
      i++;
    }
  }
  ImGui::End();

  if (ImGui::Begin("Modele")) {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, ImU32(0xffaa5511));
    ImGui::PushStyleColor(ImGuiCol_Button, ImU32(0x00000000));
    font1->Scale = 0.75;
    ImGui::PushFont(font1);
    int i = 0;
    float width_sum = 0;
    for (const auto &[name, model] : model_mgr.models) {
      ImGui::BeginGroup();

      ImGui::PushID(i);
      if (ImGui::ImageButton("##preview", (ImTextureID)model.target.texture.id, ImVec2(75, 75), ImVec2(0, 1), ImVec2(1, 0))) {
        objects.push_back(model_mgr.take_model(name, objects.size()));
      }
      ImGui::PopID();

      ImGui::SetNextItemWidth(80);
      ImGui::LabelText("##", name.c_str());
      ImGui::EndGroup();

      if (ImGui::GetContentRegionAvail().x - width_sum > 180 - 5) { // 2 elements - 5px
        ImGui::SameLine();
        width_sum += 90;
      } else {
        width_sum = 0;
      }
      i++;
    }
    font1->Scale = 1;
    ImGui::PopFont();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(2);
  }
  ImGui::End();
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
  size_t i = 0;
  for (const auto &object : objects) {
    // clang-format off
    Vector4 id
    {
      ((float)((i+1) & 0x00FF0000)) * (1.0/256.0) * (1.0/256.0) * (1.0/256.0),
      ((float)((i+1) & 0x0000FF00)) * (1.0/256.0) * (1.0/256.0),
      ((float)((i+1) & 0x000000FF)) * (1.0/256.0),
      0
    };
    // clang-format on
    SetShaderValue(assets.colorpicker_shader, location_id, &id, SHADER_UNIFORM_VEC4);
    // for (int i = 0; i < object.materialCount; i++) {
    //   object.materials[i].shader = assets.colorpicker_shader;
    // }
    DrawModel(*(Model*)&object, Vector3{i, i, i}, 100, WHITE);
    i++;
  }

  // DrawModelEx(testmodel, Vector3{0,0,0}, Vector3{0,1,0}, GetTime() * 100, Vector3{100, 100, 100}, WHITE);

  // for (const auto &[name, model] : model_mgr.models) {
  //   DrawModel(model.model, Vector3{0, 0, 0}, 100, WHITE);
  // }
  EndMode3D();
}

void App::cleanup() {
  rlImGuiShutdown();
  CloseWindow();
  UnloadRenderDoc();
}
