#include "FrameBuffer.hpp"
#include <cstdio>
#include <iostream>
#include <string>

namespace RR {
	FrameBuffer::FrameBuffer(int width, int height, int num_outputs) {
		GLuint framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		GLuint texture;
		RR::Texture2d txtemp(0);
		glGenTextures(num_outputs, &texture);
		this->textures.reserve(num_outputs);
		for (int i = 0; i < num_outputs; i++) {
			glBindTexture(GL_TEXTURE_2D, texture + i);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			//
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, texture, 0);

			txtemp = RR::Texture2d(texture + i);
			this->textures.push_back(std::move(txtemp));
		}

		GLuint renderbuffer;
		glGenRenderbuffers(1, &renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

		GLenum framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (framebuffer_status != GL_FRAMEBUFFER_COMPLETE) {
			char message[256] = {0};
			std::snprintf(message, 256, "Framebuffer error: %d\n", framebuffer_status);
			throw std::string(message);
		}

		this->id = framebuffer;
		this->depth_texture = renderbuffer;
	}

	FrameBuffer::~FrameBuffer() {
        if (this->id != -1) glDeleteFramebuffers(1, &this->id);
		// glBindFramebuffer(0); // bind it yourself
	}

    FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept {
		this->id = other.id;
		other.id = -1;
		other.textures = std::move(textures);
    }
    FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept {
		if (this != &other) {
			this->id = other.id;
			other.id = -1;
			other.textures = std::move(textures);
		}
		return *this;
    }
}
