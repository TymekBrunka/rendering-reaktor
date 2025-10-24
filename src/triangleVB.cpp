#include "rr.hpp"

struct triangle_vertex {
	RR::vec2 pos;
	RR::vec3 color;
};

GLint location_pos;
GLint location_color;
GLuint triangle_vertex_array;

void setup_triangle_vertex_array_attribs(const GLuint program) {
	RR_AUTOATTRIB(triangle_vertex, pos, GL_FALSE);
	RR_AUTOATTRIB(triangle_vertex, color, GL_FALSE);
}