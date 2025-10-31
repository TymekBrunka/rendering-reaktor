#pragma once
#include <glad/gl.h>

namespace RR {
	class FrameBuffer {
	public:
		GLuint id;
		GLuint texture;
		GLuint depth_texture;

		FrameBuffer(int width, int height);
	};
}