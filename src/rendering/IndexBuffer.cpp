#include "IndexBuffer.hpp"

namespace RR {
        IndexBuffer::IndexBuffer(const GLuint data[], GLsizeiptr n, GLenum usage) {
            glGenBuffers(1, &this->id);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, n * sizeof(GLuint), data, usage);
            this->length = n;
        }

        IndexBuffer::~IndexBuffer() {
            if (this->id != -1) glDeleteBuffers(1, &this->id);
        }

        IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept {
            this->id = other.id;
            this->length = other.length;
            other.id = -1;
        }

        IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {
            if (this != &other) {
                this->id = other.id;
                this->length = other.length;
                other.id = -1;
            }
            return *this;
        }

        void IndexBuffer::bind() {
            if (this->id != -1) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
        }
}
