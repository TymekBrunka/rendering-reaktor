#include "raylib.h"
#include "raymath.h"
#include <ModelMgr.hpp>
#include <cstddef>
#include <cstring>
#include <iostream>

#include <placeholder.png.hpp>
#include <skinning.fs.hpp>
#include <skinning.vs.hpp>

ModelRef::~ModelRef() {
  if (currentPose)
    delete[] currentPose;
  if (boneMatrices)
    delete[] boneMatrices;
}

ModelRef::ModelRef(const Model &model, const ModelAnimation *animations, const std::string name) {
  (Model &)*this = model; // copy base
  animations = animations;
  currentPose = new Transform[model.skeleton.boneCount];
  boneMatrices = new Matrix[model.skeleton.boneCount];
  memcpy(currentPose, model.currentPose, sizeof(Transform) * model.skeleton.boneCount);
  memcpy(boneMatrices, model.boneMatrices, sizeof(Matrix) * model.skeleton.boneCount);
}

ModelRef::ModelRef(const ModelRef &other) {
  animations = other.animations;
  currentPose = new Transform[other.skeleton.boneCount];
  boneMatrices = new Matrix[other.skeleton.boneCount];
  memcpy(currentPose, other.currentPose, sizeof(Transform) * other.skeleton.boneCount);
  memcpy(boneMatrices, other.boneMatrices, sizeof(Matrix) * other.skeleton.boneCount);
}

ModelRef &ModelRef::operator=(const ModelRef &other) {
  if (this != &other) {
    animations = other.animations;
    currentPose = new Transform[other.skeleton.boneCount];
    boneMatrices = new Matrix[other.skeleton.boneCount];
    memcpy(currentPose, other.currentPose, sizeof(Transform) * other.skeleton.boneCount);
    memcpy(boneMatrices, other.boneMatrices, sizeof(Matrix) * other.skeleton.boneCount);
  }
  return *this;
}

ModelRef::ModelRef(ModelRef &&other) noexcept {
  animations = other.animations;
  currentPose = other.currentPose;
  boneMatrices = other.boneMatrices;
  other.currentPose = nullptr;
  other.boneMatrices = nullptr;
  other.animations = nullptr;
}

ModelRef &ModelRef::operator=(ModelRef &&other) noexcept {
  if (this != &other) {
    animations = other.animations;
    currentPose = other.currentPose;
    boneMatrices = other.boneMatrices;
    other.currentPose = nullptr;
    other.boneMatrices = nullptr;
    other.animations = nullptr;
  }
  return *this;
}

// ModelRef::operator Model() {
//   return *(Model*)this;
// }

void ModelMgr::setup() {
  std::cerr << "Loading skinning shader\n";
  shader = LoadShaderFromMemory(skinning_vs_text, skinning_fs_text);
  if (!IsShaderValid(shader))
    std::cerr << "failed to load model(skinning) shaders from raylib\n";

  Image placeholder_ = {
      .data = placeholder_img_data.data,
      .width = placeholder_img_data.width,
      .height = placeholder_img_data.height,
      .mipmaps = 1,
      .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };

  placeholder_texture = LoadTextureFromImage(placeholder_);
}

ModelMgr::~ModelMgr() {
  for (auto &[name, model] : models) {
    UnloadModelAnimations(model.animations, model.animations_count);
    UnloadModel(model.model);
  }
  UnloadShader(shader);
}

void ModelMgr::unload_model(const std::string &name) {
  auto idx = models.find(name);
  if (idx != models.end()) {
    UnloadModelAnimations(models[name].animations, models[name].animations_count);
    UnloadModel(models[name].model);
    UnloadRenderTexture(models[name].target);
    models.erase(idx);
  }
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
  std::string name{(const char *)name_, size_t(filepath.size() - start)};

  AnimatedModel model{
      .animations_count = 0,
      .animations = nullptr,
      .model = LoadModel(filepath.c_str()),
  };

  if (!IsModelValid(model.model) && strncmp(&name[name.size() - 5], ".m3d", 4)) // if model is invalid and is not m3d model (raylib flags fully loaded m3d model as invalid at the time of writing this)
    return false;

  model.animations = LoadModelAnimations(filepath.c_str(), &model.animations_count);
  for (int i = 0; i < model.model.materialCount; i++) {
    model.model.materials[i].shader = shader;
    model.model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture = placeholder_texture;
  }

  Camera model_preview_camera{};
  model.target = LoadRenderTexture(100, 100);
  SetTextureFilter(model.target.texture, TEXTURE_FILTER_BILINEAR); // blurry instead of pixelated

  Vector3 bb = GetModelBoundingBox(model.model).max;
  // model_preview_camera.position = Vector3Scale(bb, 1.2);
  float furtherest_axis = bb.x;
  if (bb.y > bb.x)
    furtherest_axis = bb.y;
  if (bb.z > bb.y)
    furtherest_axis = bb.z;
  model_preview_camera.position = Vector3{furtherest_axis + 0.1f, furtherest_axis + 0.1f, furtherest_axis + 0.1f};
  model_preview_camera.up = Vector3{0, 1, 0};
  model_preview_camera.target = Vector3{0, 0, 0};
  model_preview_camera.fovy = 90;
  model_preview_camera.projection = CAMERA_PERSPECTIVE;

  BeginTextureMode(model.target);
  BeginMode3D(model_preview_camera);
  ClearBackground(BLANK);
  DrawModel(model.model, Vector3{0, 0, 0}, 1.0f, WHITE);
  EndMode3D();
  EndTextureMode();

  models[name] = model;
  return true;
}

ModelRef ModelMgr::take_model(const std::string &name, int obj_idx) {
  AnimatedModel &model = models[name];
  model.refs.insert(obj_idx);
  return ModelRef{model.model, model.animations, name};
}

void ModelMgr::notify_model_got_returned(const std::string &name, int obj_idx) {
  AnimatedModel &model = models[name];
  model.refs.erase(model.refs.find(obj_idx));
}
