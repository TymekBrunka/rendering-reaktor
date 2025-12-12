#pragma once
#include "Program.hpp"
#include <glad/gl.h>

namespace RR {
	struct image_data {
		int width;
		int height;
		int nrChannels;
		unsigned char* data;
	};

	image_data readImage(const char* filepath, int numOfChannels = 0);

	class Texture2d {
	public:
		GLuint id = -1;
		
		Texture2d(image_data& data, GLenum interp_mode = GL_LINEAR, GLenum clamp_mode = GL_CLAMP_TO_EDGE);
		Texture2d(GLuint id);
		~Texture2d();
		Texture2d(Texture2d&& other) noexcept;
		Texture2d& operator=(Texture2d&& other) noexcept;

		void bindToSlot(int slot);
		void bindToSlotAndName(Program& program, int slot, const char* uniformName);
	};
}