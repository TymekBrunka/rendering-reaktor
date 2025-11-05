#pragma once
#include <glad/gl.h>

namespace RR {
	class Texture2d {
	public:
		GLuint id;
		
		Texture2d(int width, int height, bool printOnErr);
		~Texture2d();
	}
}