#pragma once
#include "BufferBase.hpp"
#include <glad/glad.h>
#include <utility>

namespace RR {

template <typename T> class VertexBuffer : public BufferBase {
public:
  GLsizeiptr length;

  VertexBuffer(const T data[], GLsizeiptr n, GLenum usage) : BufferBase(GL_ARRAY_BUFFER, (const void **)data, n * sizeof(T), usage) { this->length = n; }

  VertexBuffer(VertexBuffer &&other) noexcept : BufferBase(std::move(other)) { this->length = other.length; }

  VertexBuffer &operator=(VertexBuffer &&other) noexcept {
    if (this != &other) {
      this->id = other.id;
      other.id = BufferBase::INVALID_ID;
      this->length = other.length;
    }
  }

  void bind() { BufferBase::bind(GL_ARRAY_BUFFER); }
};

} // namespace RR
