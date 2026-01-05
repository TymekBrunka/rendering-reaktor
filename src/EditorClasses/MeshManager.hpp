#pragma once
#include <vector>
#include "VertexBuffer.hpp"
#include "generators.h"
#include "tiny_obj_loader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace MeshManager {

struct Mesh_vertex {
	RR::vec3 pos;
	RR::vec2 uv;
	RR::vec3 normal;
};

struct Instance_data {
	int id;
	RR::vec4 _id;
	glm::mat4 transform;
};

std::vector<RR::VertexBuffer<Mesh_vertex>> meshes;
std::vector<Instance_data> instance_data;

void load_from_file(char* filename);

}