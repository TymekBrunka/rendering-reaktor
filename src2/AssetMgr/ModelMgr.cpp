#include "raylib.h"
#include "raymath.h"
#include <ModelMgr.hpp>
#include <cstddef>
#include <cstring>
#include <iostream>

#include <images.h>

ModelRef::~ModelRef() {
  if (model.currentPose != nullptr)
    delete[] model.currentPose;
  if (model.boneMatrices != nullptr)
    delete[] model.boneMatrices;
  if (model.materials != nullptr)
    delete[] model.materials;
  if (model.meshMaterial != nullptr)
    delete[] model.meshMaterial;
}

ModelRef::ModelRef(const AnimatedModel &model_, const std::string &name) {
  model = model_.model;
  texture_id = model_.target.texture.id;
  animations = model_.animations;
  animations_count = model_.animations_count;
  bounding_box = model_.bounding_box;
  this->name = name;
  if (model.skeleton.boneCount > 0) {
    model.currentPose = new Transform[model_.model.skeleton.boneCount];
    model.boneMatrices = new Matrix[model_.model.skeleton.boneCount];
    model.materials = new Material[model_.model.materialCount];
    model.meshMaterial = new int[model_.model.meshCount];
    memcpy(model.currentPose, model_.model.currentPose, sizeof(Transform) * model_.model.skeleton.boneCount);
    memcpy(model.boneMatrices, model_.model.boneMatrices, sizeof(Matrix) * model_.model.skeleton.boneCount);
    memcpy(model.materials, model_.model.materials, sizeof(Material) * model_.model.materialCount);
    memcpy(model.meshMaterial, model_.model.meshMaterial, sizeof(int) * model_.model.meshCount);
  }
}

ModelRef::ModelRef(const ModelRef &other) {
  model = other.model;
  texture_id = other.texture_id;
  animations_count = other.animations_count;
  bounding_box = other.bounding_box;
  animations = other.animations;
  name = other.name;
  if (other.model.skeleton.boneCount > 0) {
    model.currentPose = new Transform[other.model.skeleton.boneCount];
    model.boneMatrices = new Matrix[other.model.skeleton.boneCount];
    model.materials = new Material[other.model.materialCount];
    model.meshMaterial = new int[other.model.meshCount];
    memcpy(model.currentPose, other.model.currentPose, sizeof(Transform) * other.model.skeleton.boneCount);
    memcpy(model.boneMatrices, other.model.boneMatrices, sizeof(Matrix) * other.model.skeleton.boneCount);
    memcpy(model.materials, other.model.materials, sizeof(Material) * other.model.materialCount);
    memcpy(model.meshMaterial, other.model.meshMaterial, sizeof(int) * other.model.meshCount);
  }
}

ModelRef &ModelRef::operator=(const ModelRef &other) {
  if (this != &other) {
    model = other.model;
    texture_id = other.texture_id;
    animations_count = other.animations_count;
    bounding_box = other.bounding_box;
    animations = other.animations;
    name = other.name;
    if (other.model.skeleton.boneCount > 0) {
      model.currentPose = new Transform[other.model.skeleton.boneCount];
      model.boneMatrices = new Matrix[other.model.skeleton.boneCount];
      model.materials = new Material[other.model.materialCount];
      model.meshMaterial = new int[other.model.meshCount];
      memcpy(model.currentPose, other.model.currentPose, sizeof(Transform) * other.model.skeleton.boneCount);
      memcpy(model.boneMatrices, other.model.boneMatrices, sizeof(Matrix) * other.model.skeleton.boneCount);
      memcpy(model.materials, other.model.materials, sizeof(Material) * other.model.materialCount);
      memcpy(model.meshMaterial, other.model.meshMaterial, sizeof(int) * other.model.meshCount);
    }
  }
  return *this;
}

ModelRef::ModelRef(ModelRef &&other) noexcept {
  model = other.model;
  texture_id = other.texture_id;
  animations_count = other.animations_count;
  bounding_box = other.bounding_box;
  animations = other.animations;
  name = other.name;
  model.currentPose = other.model.currentPose;
  model.boneMatrices = other.model.boneMatrices;
  model.materials = other.model.materials;
  model.meshMaterial = other.model.meshMaterial;
  other.model.currentPose = nullptr;
  other.model.boneMatrices = nullptr;
  other.model.materials = nullptr;
  other.model.meshMaterial = nullptr;
}

ModelRef &ModelRef::operator=(ModelRef &&other) noexcept {
  if (this != &other) {
    model = other.model;
    texture_id = other.texture_id;
    animations_count = other.animations_count;
    bounding_box = other.bounding_box;
    animations = other.animations;
    name = other.name;
    model.currentPose = other.model.currentPose;
    model.boneMatrices = other.model.boneMatrices;
    model.materials = other.model.materials;
    model.meshMaterial = other.model.meshMaterial;
    other.model.currentPose = nullptr;
    other.model.boneMatrices = nullptr;
    other.model.materials = nullptr;
    other.model.meshMaterial = nullptr;
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

  AnimatedModel default_{.animations_count = 0, .animations = nullptr, .bounding_box = {Vector3{-0.5, -0.5, -0.5}, Vector3{0.5, 0.5, 0.5}}, .target = LoadRenderTexture(100, 100), .model = LoadModelFromMesh(GenMeshCube(1, 1, 1))};
  default_.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = placeholder_texture;

  models["default"] = default_;
  util_get_model_preview(default_.model, default_.target);

  placeholder_texture = LoadTextureFromImage(placeholder_);
}

ModelMgr::~ModelMgr() {
  if (!owns_data)
    return;

  for (auto &[name, model] : models) {
    UnloadModelAnimations(model.animations, model.animations_count);
    UnloadModel(model.model);
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
    if (model.animations)
      UnloadModelAnimations(model.animations, model.animations_count);
    UnloadModel(model.model);
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

  AnimatedModel model{
      .animations_count = 0,
      .animations = nullptr,
      .model = LoadModel(filepath.c_str()),
  };

  if (!IsModelValid(model.model) && strncmp(&name[name.size() - 4], ".m3d", 4)) // if model is invalid and is not m3d model (raylib flags fully loaded m3d model as invalid at the time of writing this)
    return false;

  model.animations = LoadModelAnimations(filepath.c_str(), &model.animations_count);
  for (int i = 0; i < model.model.materialCount; i++) {
    // model.model.materials[i].shader = shader;
    model.model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture = placeholder_texture;
  }

  model.target = LoadRenderTexture(100, 100);
  SetTextureFilter(model.target.texture, TEXTURE_FILTER_BILINEAR); // blurry instead of pixelated
  util_get_model_preview(model.model, model.target, &model.bounding_box);

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
