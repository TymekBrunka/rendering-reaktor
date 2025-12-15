#include "CubeMap.hpp"
#include "stb_image.h"
#include <iostream>

namespace RR {
	CubeMap::CubeMap(const image_data faces[]) {
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		for (unsigned int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, faces[i].width, faces[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE, faces[i].data
			);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		this->id = texture;
	}

	CubeMap::CubeMap(GLuint id) {
		this->id = id;
	}

	CubeMap::~CubeMap() {
		if (this->id != -1) glDeleteTextures(1, &this->id);
	}

	CubeMap::CubeMap(CubeMap&& other) noexcept {
		this->id = other.id;
		other.id = -1;
	}

	CubeMap& CubeMap::operator=(CubeMap&& other) noexcept {
		if (this != &other) {
			this->id = other.id;
			other.id = -1;
		}
		return *this;
	};

	void CubeMap::bindToSlot(int slot) {
		glActiveTexture(GL_TEXTURE0 + slot); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, this->id);
	}

	void CubeMap::bindToSlotAndName(Program& program, int slot, const char* uniformName) {
		glActiveTexture(GL_TEXTURE0 + slot); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, this->id);
		glUniform1i(glGetUniformLocation(program.id, uniformName), slot);
	}
}
