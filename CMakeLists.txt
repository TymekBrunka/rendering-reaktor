cmake_minimum_required(VERSION 3.23)
project(reaktory LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 20)

find_package(imguizmo CONFIG REQUIRED)

find_package(glm REQUIRED) #refuses to work normaly with find_package()

find_package(assimp REQUIRED)

add_subdirectory(src/rendering)

imgpack(cubemap2Img src/cubemap2.png cubemap2.png y 4)
imgpack(icon_data src/icon.png icon.png n 4)
imgpack(iconsImg src/icons.png icons.png y 4)
bytepack(skybox_vertex_text src/shaders/skybox.vertex.glsl skybox.vertex.glsl)
bytepack(skybox_fragment_text src/shaders/skybox.frag.glsl skybox.frag.glsl)

# add_library(tinyobjloader OBJECT src/tinyobjloader/tiny_obj_loader.cc)
# target_include_directories(tinyobjloader PUBLIC src/tinyobjloader/)

add_library(pfd STATIC src/pfd/pfd.cpp)
# target_include_directories(pfd PUBLIC src/pfd/)
target_precompile_headers(pfd PUBLIC src/pfd/portable-file-dialogs.h)

add_executable(main
  src/main.cpp
  src/EditorClasses/Camera.cpp
  src/EditorClasses/EditorActionsPanel.cpp
  src/EditorClasses/MeshManager.cpp
  src/utils/Logger.hpp
)

target_include_directories(main PUBLIC src/EditorClasses/ src)
target_link_libraries(main PUBLIC
  rr
  glm::glm-header-only
  # tinyobjloader
  assimp::assimp

#prebuilt assets
  cubemap2Img
  icon_data
  iconsImg
  skybox_vertex_text
  skybox_fragment_text
  INTERFACE
  pfd
)
target_precompile_headers(main REUSE_FROM pfd)
target_link_libraries(main PUBLIC imgui imguizmo::imguizmo)
