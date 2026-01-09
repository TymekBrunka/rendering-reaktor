#pragma once
#include <glad/glad.h>
#include <limits>

namespace RR {

class BufferBase {
public:
  static constexpr GLuint INVALID_ID = std::numeric_limits<GLuint>::max();
  GLuint id;

  BufferBase();
  BufferBase(GLenum type);
  BufferBase(GLenum type, const void *data[], GLsizeiptr amountOfBytes, GLenum usage);
  ~BufferBase();
  BufferBase(BufferBase &&other) noexcept;
  BufferBase &operator=(BufferBase &&other) noexcept;
  void bind(GLenum type);
};

} // namespace RR
