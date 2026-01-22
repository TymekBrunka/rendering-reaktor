#pragma once
#include "Program.hpp"
#include <limits>
#include <initializer_list>

namespace RR {

enum class AttribKind {
  FLOAT = 0,
  VEC2,
  VEC3,
  VEC4,
  MAT3 = 4,
  MAT4,
};

struct AttribSpec {
  const GLint size;
  const GLenum type = GL_FLOAT;
};

extern AttribSpec ATTRIB_SPECS[];

struct VertexArrayAttrib {
  const char *name;
  const AttribKind kind;
  GLenum normalized;
  const int offset;
};

class VertexArray {
public:
  static constexpr GLuint INVALID_ID = std::numeric_limits<GLuint>::max();
  GLuint id;

  VertexArray();
  VertexArray(const char* x);
  void setStructure(Program &program, int struct_size, std::initializer_list<VertexArrayAttrib> attribs);
  ~VertexArray();
  VertexArray(VertexArray &&other) noexcept;
  VertexArray &operator=(VertexArray &&other) noexcept;
  void bind();
};

} // namespace RR
