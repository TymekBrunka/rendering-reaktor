#include "rr.hpp"

struct triangle_vertex {
	glm::vec2 pos;
	glm::vec3 color;
};

class TriangleVertexBuffer : public RR::VertexBuffer {
	GLint location_pos;
	GLint location_color;
    
public:
    TriangleVertexBuffer(GLuint program, const void* data, GLsizeiptr n, GLenum usage): RR::VertexBuffer(program, data, n, usage) {};
	void setup_attributes(GLuint program) {
        std::cout << "hello\n";
		RR_AUTOATTRIB(triangle_vertex, pos, GL_TRUE);
		RR_AUTOATTRIB(triangle_vertex, color, GL_TRUE);
	}
};
