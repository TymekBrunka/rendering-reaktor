cmake_minimum_required(VERSION 3.23)
project(rr LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 20)

# find_package(glfw3 REQUIRED) #glfw3 is name OF THE FILE, NOT TARGET

include_directories(.)

add_library(stb_image stb_image.cpp)

add_library(rr STATIC rr.cpp
  BufferBase.cpp
	Shader.cpp
	Program.cpp
	VertexBuffer.hpp
	FrameBuffer.cpp
	Texture2d.cpp
	IndexBuffer.cpp
  VertexArray.cpp
)

target_link_libraries(rr PUBLIC glfw stb_image imgui glad)

#target_include_directories(rr PUBLIC glad/include)
target_include_directories(rr PUBLIC .)

#---- asset packers

add_executable(imgpacker assetPackers/imagePacker.cpp stb_image.cpp)
add_executable(bytepacker assetPackers/bytePacker.cpp)

function(imgpack name input output flip channels)
	add_custom_command(
		OUTPUT ${output}.hpp ${output}.cpp
		COMMAND imgpacker ${name} ${CMAKE_CURRENT_SOURCE_DIR}/${input} ${output} ${channels} ${flip}

		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${ARGV1} imgpacker
		VERBATIM
	)
	add_custom_target(${name}_file DEPENDS ${output}.cpp)
	add_library(${name} STATIC ${output}.cpp)
	add_dependencies(${name} ${name}_file)
	target_link_libraries(${name} glad stb_image)
	target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/rendering/ ${CMAKE_BINARY_DIR})

endfunction(imgpack)

function(bytepack name input output)
	add_custom_command(
		OUTPUT ${output}.hpp ${output}.cpp
		COMMAND bytepacker ${name} ${CMAKE_CURRENT_SOURCE_DIR}/${input} ${output}

		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${ARGV1} bytepacker
		VERBATIM
	)
	add_custom_target(${name}_file DEPENDS ${output}.cpp)
	add_library(${name} STATIC ${output}.cpp)
	add_dependencies(${name} ${name}_file)
	target_include_directories(${name} PUBLIC ${CMAKE_BINARY_DIR})

endfunction(bytepack)
