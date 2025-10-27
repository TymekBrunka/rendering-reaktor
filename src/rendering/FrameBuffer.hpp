#include <glad/gl.h>

namespace RR {
	class FrameBuffer {
		GLuint framebuffer;
		GLuint texture;
		GLuint depth_texture;
	public:
		FrameBuffer(GLsizei width, GLsizei height);
	};
}