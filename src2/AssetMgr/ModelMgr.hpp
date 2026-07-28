#pragma once
#include <cstddef>
#include <raylib.h>
#include <rlModels.h>
#include <string>
#include <unordered_map>
#include <unordered_set>

struct AnimatedModel {
  BoundingBox bounding_box;
  RenderTexture target;

  rlmModelAnimationSet animations;
  std::unordered_set<int> refs;
  rlmModel model = {0};
};

class ModelRef {
public:
  int texture_id; // for model preview inside objects panel
  bool owns_model = true;
  BoundingBox bounding_box;
  std::string name;
  rlmModel model = {0};
  rlmAnimatedModelInstance anim_inst;

  ModelRef() = default;
  // ~ModelRef();
  ModelRef(AnimatedModel &model_, const std::string &name);
  ModelRef(ModelRef &other);
  ModelRef &operator=(ModelRef &other);
  ModelRef(ModelRef &&other) noexcept;
  ModelRef &operator=(ModelRef &&other) noexcept;
  // operator Model();
};

class ModelMgr {
private:
  bool owns_data = true;
  Texture2D placeholder_texture;

public:
  std::unordered_map<std::string, AnimatedModel> models;

  ModelMgr() = default;
  ~ModelMgr();
  ModelMgr(ModelMgr &&other) noexcept;
  ModelMgr &operator=(ModelMgr &&other) noexcept;

  void setup();

  bool load_model(const std::string &filepath);
  void unload_model(const std::string &name);
  void util_get_model_preview(Model model, RenderTexture target, BoundingBox *bounding_box = nullptr);

  AnimatedModel &get_model(const std::string &name);
  ModelRef take_model(const std::string &name, int obj_idx);
  void notify_model_got_returned(const std::string &name, int obj_idx);
};
