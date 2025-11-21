#pragma once
#include <glad/gl.h>

namespace RR {
    template<typename T>
    class VertexBuffer {
    public:
        GLuint id = -1;
        GLsizeiptr length;

        VertexBuffer(const T data[], GLsizeiptr n, GLenum usage) {
            glGenBuffers(1, &this->id);
            glBindBuffer(GL_ARRAY_BUFFER, this->id);
            glBufferData(GL_ARRAY_BUFFER, n * sizeof(T), data, usage);
            this->length = n;
        }

        ~VertexBuffer() {
            if (this->id != -1) glDeleteBuffers(1, &this->id);
        }

        VertexBuffer(VertexBuffer&& other) noexcept {
            this->id = other.id;
            this->length = other.length;
            other.id = -1;
        }

        VertexBuffer& operator=(VertexBuffer&& other) noexcept {
            if (this != &other) {
                this->id = other.id;
                this->length = other.length;
                other.id = -1;
            }
            return *this;
        }
    };
}
