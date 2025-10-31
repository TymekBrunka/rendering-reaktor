#include <glad/gl.h>
#include <iostream>
#include "FrameBuffer.hpp"

namespace RR {
	FrameBuffer::FrameBuffer(int width, int height) {
		GLuint framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		GLuint texture;		
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		//
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

		GLuint renderbuffer;
		glGenRenderbuffers(1, &renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

		GLenum framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (framebuffer_status != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Framebuffer error: " << framebuffer_status << "\n";
		}
		// return ret;

		this->id = framebuffer;
		this->texture = texture;
		this->depth_texture = renderbuffer;
	}
}