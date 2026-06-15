#include "raylib.h"
#include "raymath.h"
#include <ModelMgr.hpp>
#include <cstddef>
#include <cstring>

ModelMgr::~ModelMgr() {
  for (auto &[name, model] : models) {
    UnloadModelAnimations(model.animations, model.animations_count);
    UnloadModel(model.model);
  }
}

void ModelMgr::unload_model(const std::string &name) {
  UnloadModelAnimations(models[name].animations, models[name].animations_count);
  UnloadModel(models[name].model);
  auto idx = models.find(name);
  if (idx != models.end())
    models.erase(idx);
}

std::string ModelMgr::load_model(const char* filepath) {
  // i just hate the c++ way to split path string by last separator
  int start = 0;
  int filepath_size = strlen(filepath);
  for (int i = filepath_size - 1; i >= 0; i--) {
    if (filepath[i] == '/' ||
#ifdef _WIN32
        filepath[i] == '\\'
#endif
    ) {
      start = i;
      break;
    }
  }

  char *name_ = new char[filepath_size - start + 1];
  memcpy(name_, &filepath[start + 1], filepath_size - start);
  std::string name{(const char *)name_, size_t(filepath_size - start)};

  AnimatedModel model{
      .model = LoadModel(filepath),
      .animations = nullptr,
      .animations_count = 0,
  };
  model.animations = LoadModelAnimations(filepath, &model.animations_count);
  models[name] = model;

  return name;
}
