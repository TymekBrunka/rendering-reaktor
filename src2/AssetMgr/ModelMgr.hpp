#pragma once
#include <cstddef>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <unordered_set>

class ModelRef : Model {
public:
  ModelAnimation *animations;
  std::string name;

  ModelRef() = default;
  ~ModelRef();
  ModelRef(const Model &model, const ModelAnimation *animations, const std::string name);
  ModelRef(const ModelRef &other);
  ModelRef &operator=(const ModelRef &other);
  ModelRef(ModelRef &&other) noexcept;
  ModelRef &operator=(ModelRef &&other) noexcept;
  // operator Model();
};

struct AnimatedModel {
  int animations_count;
  ModelAnimation *animations;
  RenderTexture target;

  std::unordered_set<int> refs;
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

  AnimatedModel &get_model(const std::string &name);
  ModelRef take_model(const std::string &name, int obj_idx);
  void notify_model_got_returned(const std::string &name, int obj_idx);
};
