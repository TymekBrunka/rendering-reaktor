#include "SDL3/SDL_dialog.h"
#include <App.hpp>
#include <ImGuizmo.h>
#include <SDL3/SDL.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <imgui.h>
#include <iostream>
#include <raylib.h>
#include <rcamera.h>
#include <rlImGui.h>
#include <rlgl.h>

#include <FPScontroler.cpp>

// #include <GLFW/glfw3.h>
#include <IconsFontAwesome6.h>
#include <RobotoRegular.h>

// #include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#include "raymath.h"

#include <Renderdoc.cpp>

#include <icon_img.h>
#include <images.h>
#include <shaders.h>

std::mutex global_lock{};
std::string imported_zip_file{};

extern char *home_dir;

static ImFont *font1;

static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::UNIVERSAL);
static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
static bool useSnap(false);

void App::select(int idx) {
  selected_object = idx;
  if (idx <= -1)
    return;
  glm::mat4x4 rltransform = *(glm::mat4x4 *)&objects[selected_object].model_ref.model.transform;
  selected_object_transform = glm::transpose(rltransform);
}

bool App::initialise() {
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

  model_mgr.setup();

  char rootdir[1024] = {0};
  snprintf(rootdir, 1024,
           "%s"
#ifdef _WIN32
           "\\"
#else
           "/"
#endif
           "%s",
           home_dir, ".reaktory");
  if (!load_app(false, rootdir))
    return false;

  Image icon_ = {
      .data = (void *)icon_png_pixels,
      .width = icon_png_width,
      .height = icon_png_height,
      .mipmaps = 1,
      .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };

  Image icons_ = {
      .data = (void *)icons_png_pixels,
      .width = icons_png_width,
      .height = icons_png_height,
      .mipmaps = 1,
      .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };

  Image skybox_ = {
      .data = (void *)skybox_png_pixels,
      .width = skybox_png_width,
      .height = skybox_png_height,
      .mipmaps = 1,
      .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };

  SetWindowIcon(icon_);

  assets.icon = LoadTextureFromImage(icon_);
  assets.icons = LoadTextureFromImage(icons_);

  color_target = LoadRenderTexture(800, 600);
  object_selection_target = LoadRenderTexture(800, 600);
  SetTextureFilter(color_target.texture, TEXTURE_FILTER_POINT);
  SetTextureFilter(object_selection_target.texture, TEXTURE_FILTER_POINT);

  Mesh cube = GenMeshCube(1, 1, 1);
  preview_box = LoadModelFromMesh(cube);
  skybox = LoadModelFromMesh(cube);
  skybox.materialCount = 1;

  std::cerr << "Loading skybox shader\n";
  Shader skybox_shader = LoadShaderFromMemory((const char *)skybox_vertex_glsl_data, (const char *)skybox_frag_glsl_data);
  skybox.materials[0].shader = skybox_shader;

  int envmap_map[1] = {MATERIAL_MAP_CUBEMAP};
  SetShaderValue(skybox_shader, GetShaderLocation(skybox_shader, "environmentMap"), &envmap_map, SHADER_UNIFORM_INT);

  skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(skybox_, CUBEMAP_LAYOUT_AUTO_DETECT);
  SetTextureFilter(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture, TEXTURE_FILTER_POINT); // pixelated instead of blurry
                                                                                                  //
  std::cerr << "Loading model(skinning) shader\n";
  assets.skinning_shader = LoadShaderFromMemory((const char *)skinning_vs_data, (const char *)skinning_fs_data);
  if (!IsShaderValid(assets.skinning_shader))
    std::cerr << "failed to load model(skinning) shader\n";

  std::cerr << "Loading model(skinning+colorpicker) shader\n";
  assets.colorpicker_shader = LoadShaderFromMemory((const char *)skinning_vs_data, (const char *)skinning_colorpicker_fs_data);
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

  // rlSetBlendMode(BLEND_ALPHA);

  camera.fovy = 90.0f;
  camera.projection = CAMERA_PERSPECTIVE;
  camera.position = Vector3{
      body.position.x,
      body.position.y + 1,
      body.position.z,
  };

  updateCamera();

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second

  return true;
}

void App::run() {
  bool mouse_locked = false;
  Vector2 last_mouse_pos = Vector2{0, 0};
  while (!WindowShouldClose()) {
    // process file dialog actions
    SDL_PumpEvents();
    global_lock.lock();
    {

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
    }
    global_lock.unlock();

    // update *post-processing targets
    if (IsWindowResized()) {
      UnloadRenderTexture(color_target);
      color_target = LoadRenderTexture(GetRenderWidth(), GetRenderHeight());
      SetTextureFilter(color_target.texture, TEXTURE_FILTER_POINT);

      UnloadRenderTexture(object_selection_target);
      object_selection_target = LoadRenderTexture(GetRenderWidth(), GetRenderHeight());
      SetTextureFilter(object_selection_target.texture, TEXTURE_FILTER_POINT);
    }

#ifndef NDEBUG
    if (IsKeyPressed(KEY_F1))
      debug_mode = !debug_mode;
#endif

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

    rlImGuiBegin();
    ImGuizmo::BeginFrame();

    render_scene();

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

void App::handle_object_selection() {
  if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
    return;

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    std::cerr << "currently selected object #" << selected_object << "\n";
    render_color_scene();
    Image pixels = LoadImageFromTexture(color_target.texture);
    unsigned char *pixel = &((unsigned char *)pixels.data)[(((pixels.height - GetMouseY()) * pixels.width) + GetMouseX()) * 4];
    // std::cerr << "size: " << pixels.width * pixels.height * 4 << "\n";
    // std::cerr << "pixel (buffer pos): " << pixel - (unsigned char *)pixels.data << "\n";

    // if pixel is blank (0x00000000) then id == -1
    // clang-format off
    int id =  pixel[2] + 
             (pixel[1] * 256) + 
             (pixel[0] * 256 * 256);
    // clang-format on

    // change selected object if gizmo is not selected or no object is currently selected (so selecting ghost gizmo doesnt disallow selection)
    if (selected_object == -1 || (!ImGuizmo::IsOver() && selected_object != -1))
      select(id - 1);
    std::cerr << "currently selected object #" << selected_object << "\n";
    std::cerr << "selected object #" << id - 1 << "\n";
    std::cerr << "\n";
    UnloadImage(pixels);
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
    {9, "załaduj",
     [](App *app) {
       static const SDL_DialogFileFilter ofd_filters[] = {{"Archiwum zip (.zip)", "zip"}, {"Wszystkie pliki", "*"}};
       SDL_ShowOpenFileDialog(load_zip_callback, NULL, nullptr, ofd_filters, 2, NULL, false);
     }},

    {3, "zapisz", [](App *app) {}},

    {5, "model",
     [](App *app) {
       static const SDL_DialogFileFilter ofd_filters[] = {{"Modele obj (.obj)", "obj"}, {"Wszystkie pliki", "*"}};
       SDL_ShowOpenFileDialog(load_model_callback, &app->models_to_load, nullptr, ofd_filters, 2, NULL, true);
     }},

    {7, "arkusz", [](App *app) {}},
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
  if (ImGui::Begin("Narzędzia")) {
    rlImGuiImageSize(&assets.icon, 20, 20);
    ImGui::SameLine();
    ImGui::Text("reaktory");

    font1->Scale = 0.9;
    ImGui::PushFont(font1);
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
    font1->Scale = 1;
    ImGui::PopFont();
    ImGui::Dummy(ImVec2(0, 20));

    ImGui::Checkbox("manipulacja objektem\nw przestrzeni\nlokalnej", &is_local_space);
    if (is_local_space)
      mCurrentGizmoMode = ImGuizmo::LOCAL;
    else
      mCurrentGizmoMode = ImGuizmo::WORLD;
    ImGui::Checkbox("użyj kroku", &use_snaping);

    ImGui::BeginDisabled(!use_snaping);
    ImGui::DragFloat("krok", &snap);
    snapping = Vector3{snap, snap, snap};
    ImGui::EndDisabled();
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
        ModelRef modelRef = model_mgr.take_model(name, objects.size());
        objects.push_back({Transform{Vector3{0, 0, 0}, Quaternion{0, 0, 0, 0}, Vector3{1, 1, 1}}, std::move(modelRef)});
      }

      ImGui::SetNextItemWidth(80);
      ImGui::LabelText("##name", name.c_str());

      if (ImGui::Button("usuń")) {
        std::unordered_set<int> &objects_using_deleted_model = model_mgr.models[name].refs;
        for (int idx : objects_using_deleted_model) {
          objects[idx] = {Transform{Vector3{0, 0, 0}, Quaternion{0, 0, 0, 0}, Vector3{1, 1, 1}}, model_mgr.take_model("default", idx)};
        }
        model_mgr.unload_model(name);

        ImGui::PopID();
        ImGui::EndGroup();
        break;
      }
      ImGui::PopID();
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

  if (ImGui::Begin("Objekty")) {
    char formatted_text[16] = {0};
    ImVec2 window_padding = ImGui::GetStyle().WindowPadding;
    ImU32 active_button_bg = ImGui::GetColorU32(ImGuiCol_ButtonActive);
    ImDrawList *drawlist = ImGui::GetWindowDrawList();

    int i = 0;
    ImGui::PushStyleColor(ImGuiCol_Button, ImU32(0x00000000));
    for (const auto &object : objects) {
      if (i == selected_object) {
        ImGui::PopStyleColor(1);
        ImGui::PushStyleColor(ImGuiCol_Button, active_button_bg);
      }
      ImGui::PushID(i);
      snprintf(formatted_text, 15, "Objekt #%d", i);
      if (ImGui::Button("##object", ImVec2(ImGui::GetWindowSize().x - (window_padding.x * 2), 30))) {
        select(i);
      }
      ImVec2 pos = ImGui::GetItemRectMin();
      drawlist->AddImage((ImTextureRef)object.model_ref.texture_id, ImVec2(pos.x + 1, pos.y + 1), ImVec2(pos.x + 29, pos.y + 29), ImVec2(0, 1), ImVec2(1, 0));
      drawlist->AddText(ImVec2(pos.x + 30, pos.y + 5), IM_COL32_WHITE, formatted_text);
      ImGui::PopID();
      if (i == selected_object) {
        ImGui::PopStyleColor(1);
        ImGui::PushStyleColor(ImGuiCol_Button, 0x00000000);
      }
      i++;
    }
    ImGui::PopStyleColor(1);
  }
  ImGui::End();

  if (ImGui::Begin("Właściwości")) {
    if (selected_object != -1) {
      WorldObject &object = objects[selected_object];
      ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(selected_object_transform), (float *)&object.transform.translation, (float *)&object.transform.rotation, (float *)&object.transform.scale);

      float width = ImGui::GetWindowSize().x - (2 * ImGui::GetStyle().WindowPadding.x);
      ImGui::TextUnformatted("pozycja");
      ImGui::SetNextItemWidth(width);
      ImGui::DragFloat3("##position", (float *)&object.transform.translation, snap, 0, 0, "%.2f");
      ImGui::TextUnformatted("obrót");
      ImGui::SetNextItemWidth(width);
      ImGui::DragFloat3("##rotation", (float *)&object.transform.rotation, 15, 0, 0, "%.1f");
      ImGui::TextUnformatted("skala");
      ImGui::SetNextItemWidth(width);
      ImGui::DragFloat3("##scale", (float *)&object.transform.scale, snap, 0, 0, "%.2f");

      ImGuizmo::RecomposeMatrixFromComponents((float *)&object.transform.translation, (float *)&object.transform.rotation, (float *)&object.transform.scale, glm::value_ptr(selected_object_transform));
    }
  }
  ImGui::End();
}

void App::render_color_scene() {
  BeginTextureMode(color_target);
  BeginMode3D(camera);
  ClearBackground(BLANK);
  size_t i = 0;
  for (const auto &object : objects) {
    // clang-format off
    Vector4 id
    {
      ((float)((i+1) & 0x00FF0000)) * (1.0/256.0) * (1.0/256.0) * (1.0/256.0),
      ((float)((i+1) & 0x0000FF00)) * (1.0/256.0) * (1.0/256.0),
      ((float)((i+1) & 0x000000FF)) * (1.0/256.0),
      1
    };
    // clang-format on
    SetShaderValue(assets.colorpicker_shader, location_id, &id, SHADER_UNIFORM_VEC4);
    for (int j = 0; j < object.model_ref.model.materialCount; j++) {
      object.model_ref.model.materials[j].shader = assets.colorpicker_shader;
    }
    DrawModel(object.model_ref.model, Vector3{0, 0, 0}, 1, WHITE);
    for (int j = 0; j < object.model_ref.model.materialCount; j++) {
      object.model_ref.model.materials[j].shader = assets.skinning_shader;
    }
    i++;
  }
  EndMode3D();
  EndTextureMode();
}

void App::render_scene() {
  rlSetBlendMode(BLEND_ALPHA);
  BeginMode3D(camera);
  rlDisableBackfaceCulling();
  rlDisableDepthMask();
  DrawModel(skybox, camera.position, 1.0f, WHITE);
  rlEnableBackfaceCulling();
  rlEnableDepthMask();

  // DrawCube({-10, -15, -20}, 20, 30, 40, RED);
  for (const auto &object : objects) {
    DrawModel(object.model_ref.model, Vector3{0, 0, 0}, 1, WHITE);
  }
  EndMode3D();

  if (selected_object != -1) {
    WorldObject &object = objects[selected_object];
    BoundingBox bb = object.model_ref.bounding_box;
    Vector3 size = Vector3{fabsf(bb.max.x - bb.min.x), fabsf(bb.max.y - bb.min.y), fabsf(bb.max.z - bb.min.z)};
    // clang-format off
    preview_box.transform = MatrixMultiply(
      MatrixMultiply(
        MatrixScale(size.x, size.y, size.z),
        MatrixTranslate(bb.min.x + size.x/2.0f, bb.min.y + size.y/2.0f, bb.min.z + size.z/2.0f)
      ),
      object.model_ref.model.transform
    );
    // clang-format on
    // preview_box.transform = MatrixScale(bb.max.x + bb.min.x, bb.max.y, bb.max.z);

    // since i cant just clear depth buffer with raylib then ill render that single box to another rendertexture and DrawTexture it
    BeginTextureMode(object_selection_target);
    ClearBackground(BLANK);
    BeginMode3D(camera);
    rlDisableDepthMask();
    DrawModel(preview_box, Vector3{0, 0, 0}, 1, Color{0x00, 0xff, 0x00, 0x90});
    rlEnableDepthMask();
    EndMode3D();
    EndTextureMode();

    // clang-format off
    DrawTexturePro(
      object_selection_target.texture,
      Rectangle{0, 0, (float)object_selection_target.texture.width, (float)-object_selection_target.texture.height},
      Rectangle{0, 0, (float)object_selection_target.texture.width, (float)object_selection_target.texture.height},
      Vector2{0,0},
      0,
      WHITE
    );
    // clang-format on

    ImGuizmo::SetRect(0, 0, GetRenderWidth(), GetRenderHeight());
    // i had to make matrices myself instead of using raylib's built-in functions because the output was wrong
    // this migh be to imguizmo using different matrix spec (unlike raylib it is either row major or left-hand-sided)
    glm::mat4x4 projection = glm::perspective(glm::radians(camera.fovy), (float)GetRenderWidth() / (float)GetRenderHeight(), 0.1f, 1000.0f);
    // clang-format off
    glm::mat4x4 view = glm::lookAt(
      glm::vec3(
        camera.position.x,
        camera.position.y,
        camera.position.z
      ),

      glm::vec3(
        camera.target.x,
        camera.target.y,
        camera.target.z
      ),

      glm::normalize(glm::vec3(
        camera.up.x,
        camera.up.y,
        camera.up.z
      ))
    );
    // clang-format on

    // because of different matrix spec i transpose matrix back and forth (otherwise it skews instead of moving object)
    ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), mCurrentGizmoOperation, mCurrentGizmoMode, glm::value_ptr(selected_object_transform), NULL, use_snaping ? (float *)&snapping : NULL);
    glm::mat4x4 rltransform = glm::transpose(selected_object_transform);
    object.model_ref.model.transform = *(Matrix *)&rltransform;
  }

  handle_object_selection();

#ifndef NDEBUG
  if (debug_mode) {
    DrawTextureEx(color_target.texture, Vector2{20, 20}, 0, 0.5, WHITE);
  }
#endif
}

void App::cleanup() {
  rlImGuiShutdown();
  UnloadRenderTexture(color_target);
  UnloadRenderTexture(object_selection_target);
  UnloadTexture(assets.icon);
  UnloadTexture(assets.icons);
  UnloadModel(skybox);
  // UnloadModel(preview_box);
  CloseWindow();
  UnloadRenderDoc();
}
