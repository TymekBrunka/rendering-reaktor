#include "VertexBuffer.hpp"

namespace RR {
    template<typename T>
    VertexBuffer::VertexBuffer(GLuint program, const T data[], GLsizeiptr n, GLenum usage): length(n) {
        glGenBuffers(1, &this->vertex_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, n, data, usage);
    }
}
