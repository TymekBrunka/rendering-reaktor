#include "rr.hpp"

struct triangle_vertex {
	RR::vec2 pos;
	RR::vec3 color;
};

class TriangleVertexBuffer : public RR::VertexBuffer {
	GLint location_pos;
	GLint location_color;
    
public:
    TriangleVertexBuffer(GLuint program, const void* data, GLsizeiptr n, GLenum usage): RR::VertexBuffer(program, data, n, usage) {};
	void setup_attributes(GLuint program) {
		RR_AUTOATTRIB(triangle_vertex, pos, GL_TRUE);
		RR_AUTOATTRIB(triangle_vertex, color, GL_TRUE);
	}
};
