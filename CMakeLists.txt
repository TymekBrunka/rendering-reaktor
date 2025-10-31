cmake_minimum_required(VERSION 3.23)
project(reaktory LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 20)

add_subdirectory(nativefiledialog-extended) #refuses to work normaly with find_package()

find_package(glfw3 REQUIRED) #glfw3 is name OF THE FILE, NOT TARGET

add_subdirectory(glm) #refuses to work normaly with find_package()

add_subdirectory(src/rendering)

add_executable(main src/main.cpp)
target_link_libraries(main nfd rr glad glm::glm-header-only)
#target_include_directories(main PUBLIC )