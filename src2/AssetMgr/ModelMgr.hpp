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
public:
  std::unordered_map<std::string, AnimatedModel> models;

  ModelMgr() = default;
  ~ModelMgr();

  std::string load_model(const char *filepath);
  void unload_model(const std::string &name);
};
