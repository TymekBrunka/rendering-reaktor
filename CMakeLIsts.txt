cmake_minimum_required(VERSION 3.23)
project(reaktory LANGUAGES C CXX)

add_subdirectory(nativefiledialog-extended)

find_package(glfw3 REQUIRED)

set(RENDER_LIBS
    glfw
    rr
)

add_library(glad OBJECT glad/src/gl.c)
target_include_directories(glad PUBLIC glad/include)

add_library(rr OBJECT src/rendering/rr.cpp)
target_link_libraries(rr glfw glad)
target_include_directories(rr PUBLIC src/rendering)

add_executable(main src/main.cpp)
target_link_libraries(main glfw nfd rr glad) #glfw3 is name OF THE FILE, NOT TARGET
#target_include_directories(main PUBLIC )