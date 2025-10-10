cmake_minimum_required(VERSION 3.23)
project(reaktory LANGUAGES C CXX)

add_subdirectory(nativefiledialog-extended) #refuses to work normaly with find_package()

find_package(glfw3 REQUIRED) #glfw3 is name OF THE FILE, NOT TARGET

add_subdirectory(glm) #refuses to work normaly with find_package()

set(RENDER_LIBS
    glad
    glfw
    rr
    glm::glm-header-only
)

add_library(glad OBJECT glad/src/gl.c)
target_include_directories(glad PUBLIC glad/include)

add_library(rr OBJECT src/rendering/rr.cpp)
target_link_libraries(rr glfw glad glm)
target_include_directories(rr PUBLIC src/rendering)

add_executable(main src/main.cpp)
target_link_libraries(main PUBLIC nfd ${RENDER_LIBS})
#target_include_directories(main PUBLIC )