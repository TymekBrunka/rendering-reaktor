#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>

struct AnimatedModel {
  Model model;
  ModelAnimation *animations;
  int animations_count;
};

class ModelMgr {
private:
  Shader shader;

public:
  std::unordered_map<std::string, AnimatedModel> models;

  ModelMgr() = default;
  ~ModelMgr();

  void setup_shader();

  std::string load_model(const std::string &filepath);
  void unload_model(const std::string &name);
};
