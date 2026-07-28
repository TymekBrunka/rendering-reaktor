#include "raylib.h"
#include "raymath.h"
#include "rlModels.h"
#include "rlModels_IO.h"
#include <ModelMgr.hpp>
#include <cstddef>
#include <cstring>
#include <iostream>

#include <images.h>

ModelRef::ModelRef(AnimatedModel &model_, const std::string &name) {
  model = rlmCloneModel(model_.model);
  owns_model = true;
  texture_id = model_.target.texture.id;
  anim_inst.sequences = &model_.animations;
  anim_inst.interpolate = true;
  anim_inst.currentFrame = 0;
  anim_inst.currentPose = rlmLoadPoseFromModel(model_.model);
  bounding_box = model_.bounding_box;
  this->name = name;
}

ModelRef::ModelRef(ModelRef &other) {
  model = rlmCloneModel(other.model);
  owns_model = true;
  texture_id = other.texture_id;
  bounding_box = other.bounding_box;
  anim_inst = other.anim_inst;
  anim_inst.currentPose = rlmLoadPoseFromModel(other.model);
  name = other.name;
}

ModelRef &ModelRef::operator=(ModelRef &other) {
  if (this != &other) {
    model = rlmCloneModel(other.model);
    owns_model = true;
    texture_id = other.texture_id;
    bounding_box = other.bounding_box;
    anim_inst = other.anim_inst;
    anim_inst.currentPose = rlmLoadPoseFromModel(other.model);
    name = other.name;
  }
  return *this;
}

ModelRef::ModelRef(ModelRef &&other) noexcept {
  model = other.model;
  owns_model = true;
  texture_id = other.texture_id;
  bounding_box = other.bounding_box;
  anim_inst = other.anim_inst;
  name = std::move(other.name);
  other.owns_model = false;
}

ModelRef &ModelRef::operator=(ModelRef &&other) noexcept {
  if (this != &other) {
    model = other.model;
    owns_model = true;
    texture_id = other.texture_id;
    bounding_box = other.bounding_box;
    anim_inst = other.anim_inst;
    name = other.name;
    name = std::move(other.name);
    other.owns_model = false;
  }
  return *this;
}

// ModelRef::operator Model() {
//   return *(Model*)this;
// }

void ModelMgr::setup() {
  owns_data = true;
  Image placeholder_ = {
      .data = (void *)placeholder_png_pixels,
      .width = placeholder_png_width,
      .height = placeholder_png_height,
      .mipmaps = 1,
      .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };

  Model immodel = LoadModelFromMesh(GenMeshCube(1, 1, 1));
  immodel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = placeholder_texture;

  // clang-format off
  AnimatedModel default_{
    .bounding_box = {
      Vector3{-0.5, -0.5, -0.5},
      Vector3{0.5, 0.5, 0.5}
    },
    .target = LoadRenderTexture(100, 100),
    .animations = {
      0, nullptr
    },
    .model = rlmLoadFromModel(immodel)};
  // clang-format on

  models["default"] = default_;
  util_get_model_preview(immodel, default_.target);

  placeholder_texture = LoadTextureFromImage(placeholder_);
}

ModelMgr::~ModelMgr() {
  if (!owns_data)
    return;

  for (auto &[name, model] : models) {
    rlmUnloadAnimationSet(&model.animations);
    rlmUnloadModel(&model.model);
    std::cerr << "Unloaded model: " << name << " (destruction)\n";
  }
  // UnloadShader(shader);
  UnloadTexture(placeholder_texture);
}

ModelMgr::ModelMgr(ModelMgr &&other) noexcept {
  placeholder_texture = other.placeholder_texture;
  models = other.models;
  owns_data = other.owns_data;
  other.owns_data = false;
}

ModelMgr &ModelMgr::operator=(ModelMgr &&other) noexcept {
  if (this != &other) {
    placeholder_texture = other.placeholder_texture;
    models = other.models;
    owns_data = other.owns_data;
    other.owns_data = false;
  }
  return *this;
}

void ModelMgr::unload_model(const std::string &name) {
  if (name == "default")
    return;

  auto idx = models.find(name);
  if (idx != models.end()) {
    AnimatedModel &model = models[name];
    if (model.animations.sequenceCount)
      rlmUnloadAnimationSet(&model.animations);
    rlmUnloadModel(&model.model);
    UnloadRenderTexture(model.target);
    std::cerr << "Unloaded model: " << name << "\n";
    models.erase(idx);
  }
}

void ModelMgr::util_get_model_preview(Model model, RenderTexture target, BoundingBox *bounding_box) {
  Camera model_preview_camera{};

  BoundingBox bb = GetModelBoundingBox(model);
  if (bounding_box)
    *bounding_box = bb;
  // model_preview_camera.position = Vector3Scale(bb, 1.2);
  float furtherest_axis = (bb.max.x - bb.min.x) / 2.0f;
  float len_y = (bb.max.y - bb.min.y) / 2.0f;
  float len_z = (bb.max.z - bb.min.z) / 2.0f;
  if (len_y > furtherest_axis)
    furtherest_axis = len_y;
  if (len_z > furtherest_axis)
    furtherest_axis = len_z;
  model_preview_camera.position = Vector3{furtherest_axis + 0.1f, furtherest_axis + 0.1f, furtherest_axis + 0.1f};
  model_preview_camera.position = Vector3Add(model_preview_camera.position, Vector3Scale(Vector3Add(bb.min, bb.max), 0.5));
  model_preview_camera.up = Vector3{0, 1, 0};
  model_preview_camera.target = Vector3{0, 0, 0};
  model_preview_camera.fovy = 90;
  model_preview_camera.projection = CAMERA_PERSPECTIVE;

  BeginTextureMode(target);
  BeginMode3D(model_preview_camera);
  ClearBackground(BLANK);
  DrawModel(model, Vector3{0, 0, 0}, 1.0f, WHITE);
  EndMode3D();
  EndTextureMode();
}

bool ModelMgr::load_model(const std::string &filepath) {
  // i just hate the c++ way to split path string by last separator
  int start = 0;
  for (int i = filepath.size() - 1; i >= 0; i--) {
    if (filepath[i] == '/'
#ifdef _WIN32
        || filepath[i] == '\\'
#endif
    ) {
      start = i;
      break;
    }
  }

  char *name_ = new char[filepath.size() - start + 1];
  memcpy(name_, &filepath[start + 1], filepath.size() - start);
  // std::string name{(const char *)name_, size_t(filepath.size() - start)};
  std::string name{(const char *)name_};
  delete[] name_;

  Model immodel = LoadModel(filepath.c_str());

  for (int i = 0; i < immodel.materialCount; i++) {
    // model.model.materials[i].shader = shader;
    immodel.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture = placeholder_texture;
  }

  AnimatedModel model{
      .model = rlmLoadFromModel(immodel),
  };

  // bool is_valid = IsModelValid(model.model);
  // if (!is_valid && strncmp(&name[name.size() - 4], ".m3d", 4)) { // if model is invalid and is not m3d model (raylib flags fully loaded m3d model as invalid at the time of writing this)
  //   UnloadModel(model.model);
  //   std::cerr << "Failed to load model (is invalid: " << !is_valid << ")\n";
  //   return false;
  // }

  ModelAnimation *animations = LoadModelAnimations(filepath.c_str(), &model.animations.sequenceCount);
  model.animations.sequences = rlmLoadModelAnimations(model.model.skeleton, animations, model.animations.sequenceCount);

  model.target = LoadRenderTexture(100, 100);
  SetTextureFilter(model.target.texture, TEXTURE_FILTER_BILINEAR); // blurry instead of pixelated
  util_get_model_preview(immodel, model.target, &model.bounding_box);

  models[name] = model;
  std::cerr << "Loaded new model: " << name << "\n";
  return true;
}

ModelRef ModelMgr::take_model(const std::string &name, int obj_idx) {
  AnimatedModel &model = models[name];
  model.refs.insert(obj_idx);
  ModelRef modelRef = ModelRef{model, name};
  return modelRef;
}

void ModelMgr::notify_model_got_returned(const std::string &name, int obj_idx) {
  AnimatedModel &model = models[name];
  model.refs.erase(model.refs.find(obj_idx));
}
