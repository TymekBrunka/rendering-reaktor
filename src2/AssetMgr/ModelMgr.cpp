#include "raylib.h"
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

void ModelMgr::load_model(const std::string &filepath) {
  // i just hate the c++ way to split path string by last separator
  int start = 0;
  for (int i = filepath.size() - 1; i >= 0; i--) {
    if (filepath[i] == '/' ||
#ifdef _WIN32
        filepath[i] == '\\'
#endif
    ) {
      start = i;
      break;
    }
  }

  char* name_ = new char[filepath.size() - start + 1];
  memcpy(name_, &filepath[start+1], filepath.size() - start);
  std::string name{(const char*)name_, size_t(filepath.size() - start)};

  AnimatedModel model{.model = LoadModel(filepath.c_str()), .animations = nullptr, .animations_count = 0};
  model.animations = LoadModelAnimations(filepath.c_str(), &model.animations_count);
  models[name] = model;
}
