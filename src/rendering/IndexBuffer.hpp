#pragma once
#include <glad/gl.h>

namespace RR {
    class IndexBuffer {
    public:
        GLuint id = -1;
        GLsizeiptr length;

        IndexBuffer(const GLuint data[], GLsizeiptr n, GLenum usage);
        ~IndexBuffer();
        IndexBuffer(IndexBuffer&& other) noexcept;
        IndexBuffer& operator=(IndexBuffer&& other) noexcept;

        void bind();
    };
}
