#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>

class TextureMgr {
  public:
    std::unordered_map<std::string, Texture2D> textures;
}
