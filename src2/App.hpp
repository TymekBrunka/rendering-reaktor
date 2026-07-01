#pragma once
#include "raylib.h"
#include <cstdint>
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

#define MAX_SPEED 75.0f
#define MAX_ACCEL 125.0f
// Grounded drag
#define FRICTION 0.86f
// Increasing air drag, increases strafing speed
// #define AIR_DRAG 0.98f
#define AIR_DRAG 0.97f
// Responsiveness for turning movement direction to looked direction
#define CONTROL 10.0f
#define VERTICAL_SPEED 30.0f

class App {
public:
  int selected_object = -1;
  int location_id = 0;
  float headTimer = 0.0f;
  float walkLerp = 0.0f;
  Vector2 sensitivity = {0.003f, 0.003f};
  Vector2 orientation = {0};
  Vector2 lean = {0};
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

public:
  struct {
    Vector3 position = {0};
    Vector3 velocity = {0};
    Vector3 dir = {0};
  } body;
  // std::vector<uint16_t> sparse_texture_list;
  std::vector<Texture2D> static_textures;
  std::vector<Gif> dynamic_textures;
  ModelMgr model_mgr;
  std::vector<WorldObject> objects;
  // std::vector<Model> objects;
  int new_obj_id = 0;

  Model skybox;
  std::vector<std::string> models_to_load;

  App() = default;
  void initialise();
  void run();
  void cleanup();

  void handle_object_selection();
  void panel_ui();
  void render_scene();
  void render_color_scene();

  void updateCamera();
  void updateBody();

  bool IconButton(const char *label, int idx = 1, ImVec2 size = ImVec2(30, 30));

  bool import_scene_zip(const char *fielpath);
};
