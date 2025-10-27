#include <glad/gl.h>

namespace RR {
    template<typename T>
    class VertexBuffer {
        GLuint vertex_buffer_id;
    public:
        GLsizeiptr length;

        VertexBuffer(GLuint program, const T data[], GLsizeiptr n, GLenum usage) {
            glGenBuffers(1, &this->vertex_buffer_id);
            glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
            glBufferData(GL_ARRAY_BUFFER, n * sizeof(T), data, usage);
            this->length = n;
        }
    };
}
