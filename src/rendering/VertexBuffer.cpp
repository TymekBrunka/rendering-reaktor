#include "VertexBuffer.hpp"

namespace RR {
    VertexBuffer::VertexBuffer(GLuint program, const void* data, GLsizeiptr n, GLenum usage): length(n) {
        glGenBuffers(1, &this->vertex_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, n, data, usage);

        glGenVertexArrays(1, &this->vertex_array_id);
        glBindVertexArray(this->vertex_array_id);
        setup_attributes(program);
    }

    void VertexBuffer::setup_attributes(GLuint program) {};

    void VertexBuffer::Apply() {
        glBindVertexArray(this->vertex_array_id);
    }
}
