#include "Texture2d.hpp"
#include "stb_image.h"

namespace RR {
	image_data readImage(const char* filepath, int numOfChannels) {
		image_data img = {0};
		unsigned char *data = stbi_load(filepath, &img.width, &img.height, &img.nrChannels, numOfChannels);
		img.data = data;
		return img;
	}

	Texture2d::Texture2d(image_data& data, GLenum interp_mode, GLenum clamp_mode) {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		//paramethers MUST BE SET to load the texture
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp_mode);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp_mode);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interp_mode);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interp_mode);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data);
		this->id = texture;
	}

	Texture2d::Texture2d(GLuint id) {
		this->id = id;
	}

	Texture2d::~Texture2d() {
		if (this->id != -1) glDeleteTextures(1, &this->id);
	}

	Texture2d::Texture2d(Texture2d&& other) noexcept {
		this->id = other.id;
		other.id = -1;
	}

	Texture2d& Texture2d::operator=(Texture2d&& other) noexcept {
		if (this != &other) {
			this->id = other.id;
			other.id = -1;
		}
		return *this;
	};

	void Texture2d::bindToSlot(int slot) {
		glActiveTexture(GL_TEXTURE0 + slot); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, this->id);
	}

	void Texture2d::bindToSlotAndName(Program& program, int slot, const char* uniformName) {
		glActiveTexture(GL_TEXTURE0 + slot); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, this->id);
		glUniform1i(glGetUniformLocation(program.id, uniformName), slot);
	}
}
