cmake_minimum_required(VERSION 3.25)
project(reaktory LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 20)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  # if (DEFINED CMAKE_STATIC_LINKER_FLAGS OR CMAKE_STATIC_LINKER_FLAGS)
  #   set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS},")
  # endif()
    # set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fsanitize=undefined -fsanitize-undefined-trap-on-error")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()

if (CMAKE_SYSTEM_NAME MATCHES "Windows" AND CMAKE_BUILD_TYPE STREQUAL "Release")
  # bc of windows using different main
  if (DEFINED CMAKE_CXX_FLAGS AND NOT CMAKE_CXX_FLAGS STREQUAL "")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS},")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-Wl,-subsystem,windows")
endif()

include(deps/ext.cmake)

add_library(static_assets src2/StaticAssets.cpp)
target_include_directories(static_assets PRIVATE src2 PUBLIC src2/embeded)

add_library(ModelMgr
  src2/AssetMgr/ModelMgr.cpp
  src2/AssetMgr/ModelMgr.hpp
)
target_include_directories(ModelMgr PRIVATE src2/AssetMgr)
target_link_libraries(ModelMgr PUBLIC raylib PRIVATE static_assets)

if (CMAKE_SYSTEM_NAME MATCHES "Windows" AND CMAKE_BUILD_TYPE STREQUAL "Release")
  add_executable(main WIN32 src2/main.cpp src2/App.cpp src2/App.hpp)
  target_compile_options(main PRIVATE -static)
  target_link_options(main PRIVATE -static)
else()
add_executable(main src2/main.cpp src2/App.cpp src2/App.hpp)
endif()

target_link_libraries(main
  raylib
  raygizmo
  rlimgui
  imgui
  SDL3::SDL3-static
  xlsxio_write_STATIC

  static_assets
  ModelMgr
)

target_include_directories(main PRIVATE src2)

message(${CMAKE_CURRENT_SOURCE_DIR})
message(${CMAKE_SOURCE_DIR})
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(main PRIVATE -fsanitize=undefined -fsanitize-undefined-trap-on-error)
  target_link_options(main PRIVATE -fsanitize=undefined -fsanitize-undefined-trap-on-error)
endif()

# add_subdirectory(src/rendering)
#
# imgpack(cubemap2Img src/cubemap2.png cubemap2.png y 4)
# imgpack(icon_data src/icon.png icon.png n 4)
# imgpack(iconsImg src/icons.png icons.png y 4)
# bytepack(skybox_vertex_text src/shaders/skybox.vertex.glsl skybox.vertex.glsl)
# bytepack(skybox_fragment_text src/shaders/skybox.frag.glsl skybox.frag.glsl)
# bytepack(model_vertex_text src/shaders/model.vertex.glsl model.vertex.glsl)
# bytepack(model_fragment_text src/shaders/model.frag.glsl model.frag.glsl)
# bytepack(composite_vertex_text src/shaders/composite.vertex.glsl composite.vertex.glsl)
# bytepack(composite_fragment_text src/shaders/composite.frag.glsl composite.frag.glsl)
#
# # add_library(tinyobjloader OBJECT src/tinyobjloader/tiny_obj_loader.cc)
# # target_include_directories(tinyobjloader PUBLIC src/tinyobjloader/)
#
# add_library(pfd STATIC src/pfd/pfd.cpp)
# # target_include_directories(pfd PUBLIC src/pfd/)
# target_precompile_headers(pfd PUBLIC src/pfd/portable-file-dialogs.h)
#
# add_executable(main
#   src/main.cpp
#   src/EditorClasses/Camera.cpp
#   src/EditorClasses/EditorActionsPanel.cpp
#   src/EditorClasses/MeshManager.cpp
#   src/utils/Logger.hpp
# )
#
# target_include_directories(main PUBLIC src/EditorClasses/ src)
# target_link_libraries(main PUBLIC
#   rr
#   glm::glm-header-only
#   # tinyobjloader
#   assimp::assimp
#
# #prebuilt assets
#   cubemap2Img
#   icon_data
#   iconsImg
#   skybox_vertex_text
#   skybox_fragment_text
#   model_vertex_text
#   model_fragment_text
#   composite_vertex_text
#   composite_fragment_text
#   INTERFACE
#   pfd
# )
# target_precompile_headers(main REUSE_FROM pfd)
# target_link_libraries(main PUBLIC imgui imguizmo)
