#include "raylib.h"
#include "raymath.h"
#include <ModelMgr.hpp>
#include <cstddef>
#include <cstring>
#include <iostream>

#include <skinning.fs.hpp>
#include <skinning.vs.hpp>

void ModelMgr::setup_shader() {
  shader = LoadShaderFromMemory(skinning_vs_text, skinning_fs_text);
  if (!IsShaderValid(shader))
    std::cerr << "failed to load model(skinning) shaders from raylib\n";
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

bool ModelMgr::load_model(const std::string& filepath) {
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
  model.animations = LoadModelAnimations(filepath.c_str(), &model.animations_count);
  // for (int i = 0; i < model.model.materialCount; i++) {
  //   model.model.materials[i].shader = shader;
  // }

  Camera model_preview_camera{};
  model.target = LoadRenderTexture(100, 100);

  Vector3 bb = GetModelBoundingBox(model.model).max;
  model_preview_camera.position = Vector3Scale(bb, 1.2);
  model_preview_camera.up = Vector3{0,1,0};
  model_preview_camera.target = Vector3{0,0,0};
  model_preview_camera.fovy = 90;
  model_preview_camera.projection = CAMERA_PERSPECTIVE;

  BeginTextureMode(model.target);
  BeginMode3D(model_preview_camera);
  ClearBackground(BLANK);
  DrawModel(model.model, Vector3{0,0,0}, 1.0f, DARKBLUE);
  EndMode3D();
  EndTextureMode();

  models[name] = model;
  return true;
}
