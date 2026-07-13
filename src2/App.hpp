#pragma once
#include "raylib.h"
#include <cstdint>
#include <glm/mat4x4.hpp>
#include <imgui.h>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <AssetMgr/ModelMgr.hpp>

extern std::mutex global_lock;
extern std::string imported_zip_file;

struct WorldObject {
  Transform transform;
  ModelRef model_ref;
};

struct Gif {
  uint16_t numOfFrames;
  uint16_t current_frame;
  float animation_speed;
  Texture2D tdynamic;
  Image data;
};

struct AppMetadata {
  bool possibly_had_lost_its_files = false;
  int app_major, app_minor, app_patch;
  int format_major, format_minor, format_patch;
};

struct SavableState {
  int selected_object = -1;
  // int new_obj_id = 0;
  std::vector<WorldObject> objects;
  ModelMgr model_mgr;

  void setup();
};

#define MAX_SPEED 75.0f
#define MAX_ACCEL 90.0f
// Grounded drag
#define FRICTION 0.86f
// Increasing air drag, increases strafing speed
// #define AIR_DRAG 0.98f
#define AIR_DRAG 0.95f
// Responsiveness for turning movement direction to looked direction
#define CONTROL 10.0f
#define VERTICAL_SPEED 15.0f

class App {
public:
#ifndef NDEBUG
  bool debug_mode = true;
#endif
  bool use_snaping = false;
  bool is_local_space = true;
  int location_id = 0;
  float snap = 0.5;

  float headTimer = 0.0f;
  float walkLerp = 0.0f;
  Vector2 sensitivity = {0.003f, 0.003f};
  Vector2 orientation = {0};
  Vector2 lean = {0};

  Vector3 snapping = {0};
  Camera camera = {0};
  struct {
    Texture2D icon;
    Texture2D icons;
    // Texture2D skybox;
    Shader colorpicker_shader;
    Shader skinning_shader;
  } assets;

private:
  RenderTexture color_target;
  RenderTexture object_selection_target;

public:
  glm::mat4x4 selected_object_transform;
  struct {
    Vector3 position = {0};
    Vector3 velocity = {0};
    Vector3 dir = {0};
  } body;
  // std::vector<Texture2D> static_textures;
  // std::vector<Gif> dynamic_textures;

  Model skybox;
  Model preview_box;
  std::vector<std::string> models_to_load;

  AppMetadata metadata;

  SavableState state;

  App() = default;
  bool initialise();
  void run();
  void cleanup();

  void select(int idx);
  void panel_ui();
  void render_scene();

  void updateCamera();
  void updateBody();

  bool IconButton(const char *label, int idx = 1, ImVec2 size = ImVec2(30, 30));

private:
  void handle_object_selection();
  void render_color_scene();
  bool read_data_txt(char *data_txt, size_t data_txt_len, AppMetadata *meta);

public:
  bool load_app(bool from_zip, const char *root);
  bool import_scene_zip(const char *fielpath);
  bool save();
};
