#include <glad/gl.h>

namespace RR {
    class VertexBuffer {
        public:
        unsigned int bufferid;

        VertexBuffer(const void* data, GLsizeiptr n, GLenum usage) {
            glGenBuffers(1, &this->bufferid);
            glBindBuffer(GL_ARRAY_BUFFER, this->bufferid);
            glBufferData(GL_ARRAY_BUFFER, n, data, usage);
        }
    };
}
