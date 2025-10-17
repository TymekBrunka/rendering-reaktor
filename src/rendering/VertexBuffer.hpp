#include <glad/gl.h>

namespace RR {
    class VertexBuffer {
        void setup_attributes(GLuint program);

        public:
        GLuint vertex_buffer_id;
        GLuint vertex_array_id;
        const GLsizeiptr length;

        VertexBuffer(GLuint program, const void* data, GLsizeiptr n, GLenum usage);
        void Apply();
    };
}
