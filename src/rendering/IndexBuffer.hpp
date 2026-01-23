#pragma once
#include "BufferBase.hpp"

namespace RR {

class IndexBuffer : public BufferBase {
public:
  GLsizeiptr length;

  IndexBuffer(const GLuint data[], GLsizeiptr n, GLenum usage);
  // ~IndexBuffer();
  IndexBuffer(IndexBuffer &&other) noexcept;
  IndexBuffer &operator=(IndexBuffer &&other) noexcept;

  void bind();
};

} // namespace RR
