#pragma once
#include <raylib.h>
#include <unordered_map>
#include <string>

struct AnimatedModel {
  Model model;
  ModelAnimation* animations;
  int animations_count;
};

class ModelMgr {
public:
  std::unordered_map<std::string, AnimatedModel> models;

  ModelMgr() = default;
  ~ModelMgr();

  void load_model(const std::string& filepath);
  void unload_model(const std::string& name);
};
