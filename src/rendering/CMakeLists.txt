cmake_minimum_required(VERSION 3.23)
project(rr LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 20)

include_directories(.)

add_library(glad OBJECT glad/src/gl.c)
target_include_directories(glad PUBLIC glad/include)
#
#add_library(shader OBJECT Shader.cpp)
#target_link_libraries(shader glad)
#
#add_library(vb OBJECT VertexBuffer.cpp)
#target_link_libraries(vb glad)
#
#add_library(fb OBJECT FrameBuffer.cpp)
#target_link_libraries(fb glad)

add_library(stb_image stb_image.cpp)

add_library(rr OBJECT rr.cpp
	Shader.cpp
	Program.cpp
	#VertexBuffer.cpp
	FrameBuffer.cpp
	Texture2d.cpp
	IndexBuffer.cpp
)

add_subdirectory(imgui)

target_link_libraries(rr PUBLIC glfw stb_image imgui glad)

#target_include_directories(rr PUBLIC glad/include)
target_include_directories(rr PUBLIC .)

#---- asset packers

add_executable(imgpacker assetPackers/imagePacker.cpp stb_image.cpp)

function(imgpack)
	add_custom_command(
		OUTPUT ${ARGV2}.hpp ${ARGV2}.cpp
		COMMAND imgpacker ${ARGV0} ${CMAKE_CURRENT_SOURCE_DIR}/${ARGV1} ${ARGV2} 4

		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${ARGV0} imgpacker
		VERBATIM
	)
endfunction(imgpack)
