#pragma once
#include <cstddef>
#include <raylib.h>
#include <string>
#include <unordered_map>

struct AnimatedModel {
  int animations_count;
  ModelAnimation *animations;
  RenderTexture target;
  Model model;
};

class ModelMgr {
private:
  Texture2D placeholder_texture;
  Shader shader;

public:
  std::unordered_map<std::string, AnimatedModel> models;

  ModelMgr() = default;
  ~ModelMgr();

  void setup();

  bool load_model(const std::string &filepath);
  void unload_model(const std::string &name);
};
