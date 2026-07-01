#pragma once
#include <cstddef>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <unordered_set>

struct AnimatedModel {
  int animations_count;
  ModelAnimation *animations;
  BoundingBox bounding_box;
  RenderTexture target;

  std::unordered_set<int> refs;
  Model model;
};

class ModelRef {
public:
  int texture_id; // for model preview inside objects panel
  int animations_count = 0;
  Model model;
  BoundingBox bounding_box;
  ModelAnimation *animations;
  std::string name;

  ModelRef() = default;
  ~ModelRef();
  ModelRef(const AnimatedModel &model_, const std::string &name);
  ModelRef(const ModelRef &other);
  ModelRef &operator=(const ModelRef &other);
  ModelRef(ModelRef &&other) noexcept;
  ModelRef &operator=(ModelRef &&other) noexcept;
  // operator Model();
};

class ModelMgr {
private:
  Texture2D placeholder_texture;

public:
  std::unordered_map<std::string, AnimatedModel> models;

  ModelMgr() = default;
  ~ModelMgr();

  void setup();

  bool load_model(const std::string &filepath);
  void unload_model(const std::string &name);
  void util_get_model_preview(Model model, RenderTexture target, BoundingBox *bounding_box = nullptr);

  AnimatedModel &get_model(const std::string &name);
  ModelRef take_model(const std::string &name, int obj_idx);
  void notify_model_got_returned(const std::string &name, int obj_idx);
};
