set(APP_MAJOR 0)
set(APP_MINOR 0)
set(APP_PATCH 1)

set(FORMAT_MAJOR 0)
set(FORMAT_MINOR 0)
set(FORMAT_PATCH 1)

cmake_minimum_required(VERSION 3.25)
project(reaktory
  LANGUAGES C CXX
  VERSION ${APP_MAJOR}.${APP_MINOR}.${APP_PATCH}
)
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
include(BuildTools.cmake)

bytepack_(
  OUTPUT shaders
  SOURCES
    assets/shaders/cubemap.vs
    assets/shaders/cubemap.fs
    assets/shaders/skinning.vs
    assets/shaders/skinning.fs
    assets/shaders/skinning_colorpicker.fs
    assets/shaders/skybox.vertex.glsl
    assets/shaders/skybox.frag.glsl
)

bytepack_(
  OUTPUT first_launch_data
  SOURCES
    assets/data.txt
    assets/data.xlsx
    assets/scene.json
    assets/images/skybox.png
)

imgpack_(
  OUTPUT images
  CHANNELS 4
  FLIP y
  SOURCES
    assets/images/icons.png
    assets/images/placeholder.png
)

imgpack_(
  OUTPUT icon_img
  CHANNELS 4
  FLIP n
  SOURCES
    assets/images/icon.png
)

add_library(pch pch.hpp pch.cpp)
target_precompile_headers(pch PUBLIC pch.hpp)

add_library(imgui_assets src2/embeded/imgui_assets.cpp)
target_include_directories(imgui_assets PRIVATE src2 PUBLIC src2/embeded)

add_library(ModelMgr
  src2/AssetMgr/ModelMgr.cpp
  src2/AssetMgr/ModelMgr.hpp
)
target_include_directories(ModelMgr PRIVATE src2/AssetMgr)
target_link_libraries(ModelMgr PUBLIC raylib PRIVATE images)
target_precompile_headers(ModelMgr REUSE_FROM pch)

if (CMAKE_SYSTEM_NAME MATCHES "Windows" AND CMAKE_BUILD_TYPE STREQUAL "Release")
  add_executable(main WIN32 src2/main.cpp src2/App.cpp src2/App.hpp src2/SaveLoad/SaveMod.cpp)
else()
  add_executable(main src2/main.cpp src2/App.cpp src2/App.hpp src2/SaveLoad/SaveMod.cpp)
endif()

# if (CMAKE_SYSTEM_NAME MATCHES "Windows" AND CMAKE_BUILD_TYPE STREQUAL "Release")
#   add_executable(main WIN32 src2/main.cpp)
# else()
#   add_executable(main src2/main.cpp)
# endif()

target_compile_options(main PRIVATE
  -DAPP_MAJOR=${APP_MAJOR}
  -DAPP_MINOR=${APP_MINOR}
  -DAPP_PATCH=${APP_PATCH}

  -DFORMAT_MAJOR=${FORMAT_MAJOR}
  -DFORMAT_MINOR=${FORMAT_MINOR}
  -DFORMAT_PATCH=${FORMAT_PATCH}
)

if (SANS)
  target_link_options(main PRIVATE -fsanitize=undefined,address)
  target_link_options(bytepack PRIVATE -fsanitize=undefined,address)
  target_link_options(imgpack PRIVATE -fsanitize=undefined,address)
endif()

if (CMAKE_SYSTEM_NAME MATCHES "Windows" OR CMAKE_BUILD_TYPE STREQUAL "Release")
  # bc otherwise it depends on compiler libs in release mode and crashes(on windows) in renderdoc
  target_compile_options(main PRIVATE -static)
  target_link_options(main PRIVATE -static)
endif()

target_link_libraries(main
  raylib
  rlimgui
  imgui
  imguizmo
  SDL3::SDL3-static
  xlsxio_write_STATIC
  libzip::zip
  glm::glm
  yyjson

  imgui_assets
  shaders
  images
  icon_img
  first_launch_data

  ModelMgr
)
target_precompile_headers(main REUSE_FROM pch)

target_include_directories(main PRIVATE src2)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(main PRIVATE -fsanitize=undefined -fsanitize-undefined-trap-on-error)
  target_link_options(main PRIVATE -fsanitize=undefined -fsanitize-undefined-trap-on-error)

  target_compile_options(bytepack PRIVATE -fsanitize=undefined -fsanitize-undefined-trap-on-error)
  target_link_options(bytepack PRIVATE -fsanitize=undefined -fsanitize-undefined-trap-on-error)

  target_compile_options(imgpack PRIVATE -fsanitize=undefined -fsanitize-undefined-trap-on-error)
  target_link_options(imgpack PRIVATE -fsanitize=undefined -fsanitize-undefined-trap-on-error)
endif()
