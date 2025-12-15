cmake_minimum_required(VERSION 3.23)
project(reaktory LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 20)

add_subdirectory(nativefiledialog-extended) #refuses to work normaly with find_package()

find_package(glfw3 REQUIRED) #glfw3 is name OF THE FILE, NOT TARGET
#find_package(imgui CONFIG REQUIRED)
#find_package(imguizmo CONFIG REQUIRED)

add_subdirectory(glm) #refuses to work normaly with find_package()

add_subdirectory(src/rendering)

add_executable(main
  src/main.cpp
  src/EditorClasses/Camera.cpp
)
target_include_directories(main PUBLIC src/EditorClasses/)
target_link_libraries(main PUBLIC nfd rr glm::glm-header-only)
#target_include_directories(main PUBLIC imgui::imgui imguizmo::imguizmo)