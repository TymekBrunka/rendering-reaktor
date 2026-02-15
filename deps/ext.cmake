include(deps/CPM.cmake)
set(CPM_USE_LOCAL_PACKAGES ON)

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
  message(found)
  set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM} base_dir=${PROJECT_SOURCE_DIR} hash_dir=false)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM} base_dir=${PROJECT_SOURCE_DIR} hash_dir=false)
endif()

# rendering
CPMAddPackage(
  NAME glfw
  VERSION 3.4
  GITHUB_REPOSITORY glfw/glfw
  GIT_TAG 3.4
  OPTIONS
    "GLFW_BUILD_EXAMPLES OFF"
    "GLFW_BUILD_TESTS OFF"
    "GLFW_BUILD_DOCS OFF"
)

add_library(glad OBJECT deps/glad/src/glad.c)
target_include_directories(glad PUBLIC deps/glad/include)

add_subdirectory(deps/imgui)

CPMAddPackage(
  NAME imguizmo
  VERSION 1.83
  GITHUB_REPOSITORY CedricGuillemet/ImGuizmo
  GIT_TAG 1.83
  DOWNLOAD_ONLY
)

#loading models
CPMAddPackage(
  NAME assimp
  VERSION 6.0.4
  GITHUB_REPOSITORY assimp/assimp
  GIT_TAG v6.0.4
  OPTIONS
    "BUILD_SHARED_LIBS OFF"
    "ASSIMP_BUILD_TESTS OFF"
    "ASSIMP_INSTALL OFF"
    "ASSIMP_BUILD_DOCS OFF"
)

#excel
CPMAddPackage(
  NAME minizip
  VERSION 4.1.0
  GITHUB_REPOSITORY zlib-ng/minizip-ng
  GIT_TAG 4.1.0
  OPTIONS
    "MZ_ICONV OFF"
)

find_package(expat)
if (NOT ${expat})
CPMAddPackage(
  NAME expat
  VERSION 2.7.4
  GITHUB_REPO libexpat/libexpat
  GIT_TAG "R_2_7_4"
  DOWNLOAD_ONLY
)

set(EXPAT_BUILD_TOOLS OFF)
set(EXPAT_SHARED_LIBS OFF)
set(EXPAT_BUILD_EXAMPLES OFF)
set(EXPAT_ENABLE_INSTALL OFF)
add_subdirectory(${libexpat_SOURCE_DIR}/expat)
endif()

CPMAddPackage(
  NAME xlsxio
  VERSION 0.2.36
  GITHUB_REPOSITORY brechtsanders/xlsxio
  GIT_TAG 0.2.36
  OPTIONS
    "CMAKE_POLICY_VERSION_MINIMUM 3.5"
    "BUILD_STATIC ON"
    "BUILD_SHARED OFF"
    "BUILD_DOCUMENTATION OFF"
    "BUILD_PC_FILES OFF"
    "BUILD_EXAMPLES OFF"
    "WITH_MINIZIP_NG ON"
)
