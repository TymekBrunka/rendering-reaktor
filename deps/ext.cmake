include(deps/CPM.cmake)
set(CPM_USE_LOCAL_PACKAGES ON)

macro (install)
endmacro ()

# macro (find_package)
# endmacro ()

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
  message("Ccache enabled")
  set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM} base_dir=${PROJECT_SOURCE_DIR} hash_dir=false)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM} base_dir=${PROJECT_SOURCE_DIR} hash_dir=false)
endif()

#math
message(glm)
CPMAddPackage(
  NAME glm
  VERSION 1.0.3
  GITHUB_REPOSITORY g-truc/glm
  GIT_TAG 1.0.3
  OPTIONS
    "GLM_ENABLE_CXX_20 ON"
)

# rendering
message(glfw)
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

message(bettergl)
CPMAddPackage(
  NAME bettergl
  GITHUB_REPOSITORY tymekbrunka/bettyergl
  GIT_TAG main
  OPTIONS
    "BETTERGL_BUILD_TEST_EXAMPLE OFF"
)

# message(glad)
# add_library(glad OBJECT deps/glad/src/glad.c)
# target_include_directories(glad PUBLIC deps/glad/include)

file(GLOB imgui_SRC
  deps/imgui/*.hpp
  deps/imgui/*.cpp
)

message(imgui)
add_library(imgui OBJECT ${imgui_SRC})
target_include_directories(imgui PUBLIC deps/imgui)
target_compile_options(imgui PRIVATE "-DIMGUI_IMPL_OPENGL_LOADER_CUSTOM <glad/glad.h>")
target_link_libraries(imgui glad glfw)

message(imguizmo)
CPMAddPackage(
  NAME imguizmo
  VERSION 1.83
  GITHUB_REPOSITORY CedricGuillemet/ImGuizmo
  GIT_TAG 1.83
  DOWNLOAD_ONLY
)

file(GLOB imguizmo_SRC
  "${imguizmo_SOURCE_DIR}/*.h"
  "${imguizmo_SOURCE_DIR}/*.cpp"
)

add_library(imguizmo "${imguizmo_SOURCE_DIR}/ImGuizmo.cpp" ${imguizmo_SRC})
target_include_directories(imguizmo PUBLIC ${imguizmo_SOURCE_DIR})
target_compile_options(imguizmo PRIVATE -DIMGUI_DEFINE_MATH_OPERATORS)
target_compile_features(imguizmo PRIVATE cxx_std_11)
target_link_libraries(imguizmo PUBLIC imgui)

#loading models

message(assimp)
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
    "ASSIMP_BUILD_ZLIB ON"
    "ZLIB_LIBRARY ${ZLIBLIB}"
    "ZLIB_INCLUDE_DIR ${ZLIB_INCLUDES}"
    "ZLIB_DIR ${zlib_SOURCE_DIR}"
)

find_package(ZLIB 1.3.1.3)
if (NOT ZLIB)
  #   CPMAddPackage( #just couse frikin assimp doesnt let other targets use zlib if compiled from source
  #     NAME zlib
  #     VERSION 1.3.1.2
  #     GITHUB_REPOSITORY madler/zlib
  #     GIT_TAG v1.3.1.2
  #     OPTIONS
  #       "ZLIB_BUILD_STATIC ON"
  #       "ZLIB_BUILD_TESTING OFF"
  #       "ZLIB_BUILD_SHARED OFF"
  #       "ZLIB_INSTALL OFF"
  #   )

  add_library(ZLIB::ZLIB ALIAS zlibstatic)
endif()

# get_target_property(ZLIBLIB $<TARGET_FILE:zlibstatic> LOCATION)
# message("zliublib " ${})

#excel
set(CPM_USE_LOCAL_PACKAGES OFF)
message(expat)
CPMAddPackage(
  NAME expat
  VERSION 2.7.4
  GITHUB_REPOSITORY libexpat/libexpat
  GIT_TAG R_2_7_4
  DOWNLOAD_ONLY
  # OPTIONS
  #   "EXPAT_BUILD_TOOLS OFF"
  #   "EXPAT_SHARED_LIBS OFF"
  #   "EXPAT_BUILD_EXAMPLES OFF"
  #   "EXPAT_ENABLE_INSTALL OFF"
)
set(CPM_USE_LOCAL_PACKAGES ON)

set(EXPAT_BUILD_TOOLS OFF)
set(EXPAT_SHARED_LIBS OFF)
set(EXPAT_BUILD_EXAMPLES OFF)
set(EXPAT_ENABLE_INSTALL OFF)

set(EXPAT_DIR "EXPAT_DIR ${expat_SOURCE_DIR}/expat")

message(STATUS expatsrc ${expat_SOURCE_DIR})
add_subdirectory(${expat_SOURCE_DIR}/expat ${CMAKE_BINARY_DIR}/expat.dir)

message(libzip)
CPMAddPackage(
  NAME libzip
  VERSION 1.11.4
  GITHUB_REPOSITORY nih-at/libzip
  GIT_TAG v1.11.4
  OPTIONS
    "ENABLE_OPENSSL OFF"
    "ENABLE_COVERAGE OFF"
    "ENABLE_ZSTD OFF"
    "ENABLE_COMMONCRYPTO OFF"
    "ENABLE_GNUTLS OFF"
    "ENABLE_MBEDTLS OFF"
    "ENABLE_BZIP2 OFF"
    "ENABLE_LZMA OFF"
    "ENABLE_FDOPEN OFF"
    "BUILD_SHARED_LIBS OFF"
    "BUILD_DOC OFF"
    "BUILD_EXAMPLES OFF"
    "BUILD_OSSFUZZ OFF"
    "BUILD_REGRESS OFF"
    "LIBZIP_DO_INSTALL OFF"
    "BUILD_SHARED_LIBS OFF"

    "CFLAGS -I${libzip_SOURCE_DIR}"
)

target_include_directories(zip PUBLIC ${libzip_SOURCE_DIR})

get_target_property(LIBZIP_INCLUDES libzip::zip INCLUDE_DIRECTORIES)
get_target_property(EXPAT_INCLUDES expat INCLUDE_DIRECTORIES)
message(STATUS expat includes ${EXPAT_INCLUDES})
get_target_property(ZLIB_INCLUDES zlibstatic INCLUDE_DIRECTORIES)
list(APPEND LIBZIP_INCLUDES $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/deps> $<BUILD_INTERFACE:${libzip_BINARY_DIR}>)

message(xlsxio)
CPMAddPackage(
  NAME xlsxio
  VERSION 0.2.36
  GITHUB_REPOSITORY brechtsanders/xlsxio
  GIT_TAG 0.2.36
  OPTIONS
    "CMAKE_POLICY_VERSION_MINIMUM 3.5"
    "BUILD_STATIC ON"
    "BUILD_SHARED OFF"
    "BUILD_TOOLS OFF"
    "BUILD_DOCUMENTATION OFF"
    "BUILD_PC_FILES OFF"
    "BUILD_EXAMPLES OFF"
    # "WITH_MINIZIP_NG ON"
    "WITH_LIBZIP ON"

    "EXPAT_INCLUDE_DIR ${EXPAT_INCLUDES}"
    "EXPAT_LIBRARIES expat"
    "EXPAT_DIR ${EXPAT_DIR}"
    # "ZLIB_INCLUDE_DIR ${ZLIB_INCLUDES}"
    # "ZLIB_DIR ${zlib_SOURCE_DIR}"

    "LIBZIP_INCLUDE_DIRS ${LIBZIP_INCLUDES}"
)

target_link_libraries(xlsxio_read_STATIC zip)
target_include_directories(xlsxio_read_STATIC PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/deps>)

target_link_libraries(xlsxio_write_STATIC zip)
target_include_directories(xlsxio_write_STATIC PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/deps>)
