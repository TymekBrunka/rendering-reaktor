#include "rr.hpp"

struct triangle_vertex {
	glm::vec2 pos;
	glm::vec3 color;
};

class TriangleVertexBuffer : RR::VertexBuffer {
	GLint location_pos;
	GLint location_color;

	void setup_attributes(GLuint program) {
		RR_AUTOATTRIB(triangle_vertex, pos, GL_TRUE);
		RR_AUTOATTRIB(triangle_vertex, color, GL_TRUE);
	}
};