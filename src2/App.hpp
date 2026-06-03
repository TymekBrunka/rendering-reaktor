#pragma once
#include <cstdint>
#include <memory>
#include <raylib.h>
#include <vector>

struct WorldObject {
  Model model;
  Texture2D texture;
  Matrix transform;
};

struct Gif {
  uint16_t numOfFrames;
  uint16_t current_frame;
  float animation_speed;
  Texture2D tdynamic;
  Image data;
};

class App {
public:
  struct {
    Texture2D icon;
    Texture2D icons;
    Texture2D skybox;
  } assets;
  struct {
    Vector3 position;
    Vector3 velocity;
    Vector3 dir;
  } Body;
  std::vector<uint16_t> sparse_texture_list;
  std::vector<Texture2D> static_textures;
  std::vector<Gif> dynamic_textures;
  std::vector<Model> models;
  std::vector<WorldObject> objects;

  App() = default;
  void initialise();
  void run();
  void cleanup();

  void panel_ui();
  void render_scene();
};
