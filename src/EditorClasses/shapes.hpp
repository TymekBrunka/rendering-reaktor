#pragma once
#include "glMathTypes.hpp"
#include <vector>

class Shape {
	RR::vec3 position;
	RR::vec2 rotation;
	RR::vec3 size;

	RR::mat4 wmatrix;
};

class Cube : Shape {

};

struct mesh_vert {
	RR::vec3 pos;
	RR::vec2 uv;
}

class Mesh : Shape {
	vector<RR::mesh_vert> verticies;
}