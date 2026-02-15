#include "FrameBuffer.hpp"
#include <cstdio>
#include <iostream>
#include <string>

#include "BufferBase.hpp"
#include "impl_buffer_move.hpp"

namespace RR {

FrameBuffer::FrameBuffer() {}

FrameBuffer::~FrameBuffer() {
  if (this->id != FrameBuffer::INVALID_ID) {
    glDeleteFramebuffers(1, &this->id);
  }
}

FrameBuffer::FrameBuffer(int width, int height, int num_outputs) : BufferBase() {
  glGenFramebuffers(1, &this->id);
  glBindFramebuffer(GL_FRAMEBUFFER, this->id);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->id);

  GLuint *texturess = new GLuint[num_outputs];
  glGenTextures(num_outputs, texturess);
  RR::Texture2d txtemp((GLuint)0);
  std::vector<GLenum> output_names;
  output_names.reserve(num_outputs);
  this->textures.reserve(num_outputs);
  for (int i = 0; i < num_outputs; i++) {
    glBindTexture(GL_TEXTURE_2D, texturess[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texturess[i], 0);

    txtemp = RR::Texture2d(texturess[i]);
    this->textures.push_back(std::move(txtemp));
    output_names.push_back(GL_COLOR_ATTACHMENT0 + i);
  }

  // without this, there will be no writes to framebuffers
  glDrawBuffers(num_outputs, output_names.data());
  delete[] texturess;

  GLuint renderbuffer;
  glGenRenderbuffers(1, &renderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

  GLenum framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (framebuffer_status != GL_FRAMEBUFFER_COMPLETE) {
    char message[256] = {0};
    std::snprintf(message, 256, "Framebuffer error: %d\n", framebuffer_status);
    throw std::string(message);
  }

  this->depth_texture = renderbuffer;
}

__RR_impl_buffer_move(FrameBuffer, GL_FRAMEBUFFER, this->textures = std::move(other.textures);)

} // namespace RR
