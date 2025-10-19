#include "VertexBuffer.hpp"

namespace RR {
    void VertexBuffer::setup_attributes(GLuint program) {};

    VertexBuffer::VertexBuffer(GLuint program, const void* data, GLsizeiptr n, GLenum usage): length(n) {
        glGenBuffers(1, &this->vertex_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, n, data, usage);

        glGenVertexArrays(1, &this->vertex_array_id);
        glBindVertexArray(this->vertex_array_id);
        // this->setup_attributes(program); // commented out bc it would only call its own  method
    }

    void VertexBuffer::Apply() {
        glBindVertexArray(this->vertex_array_id);
    }
}
