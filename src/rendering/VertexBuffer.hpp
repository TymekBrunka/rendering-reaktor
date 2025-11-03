#pragma once
#include <glad/gl.h>

namespace RR {
    template<typename T>
    class VertexBuffer {
    public:
        GLuint id;
        GLsizeiptr length;

        VertexBuffer(const T data[], GLsizeiptr n, GLenum usage) {
            glGenBuffers(1, &this->id);
            glBindBuffer(GL_ARRAY_BUFFER, this->id);
            glBufferData(GL_ARRAY_BUFFER, n * sizeof(T), data, usage);
            this->length = n;
        }

        ~VertexBuffer() {
            glDeleteBuffers(1, &this->id);
        }
    };
}
