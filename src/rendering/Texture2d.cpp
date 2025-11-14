#include "Texture2d.hpp"
#include "stb_image.h"

namespace RR {
	image_data readImage(const char* filepath) {
		image_data img = {0};
		unsigned char *data = stbi_load(filepath, &img.width, &img.height, &img.nrChannels, 0);
		img.data = data;
		return img;
	}

	Texture2d::Texture2d(image_data& data) {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		//paramethers MUST BE SET to load the texture
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data);
		this->id = texture;
	}

	Texture2d::~Texture2d() {
		glDeleteTextures(1, &this->id);
	}

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