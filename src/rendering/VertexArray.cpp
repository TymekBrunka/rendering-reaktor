#include "VertexArray.hpp"
#include <iostream>

namespace RR {

// clang-format off
AttribSpec ATTRIB_SPECS[] = {
  {1, GL_FLOAT},
  {2, GL_FLOAT},
  {3, GL_FLOAT},
  {4, GL_FLOAT},
  {9, GL_FLOAT},
  {16, GL_FLOAT},
};
// clang-format on

VertexArray::VertexArray() : id(VertexArray::INVALID_ID) {}

VertexArray::VertexArray(const char* x) {
  glGenVertexArrays(1, &this->id);
  glBindVertexArray(id);
}

void VertexArray::setStructure(Program &program, int struct_size, std::initializer_list<VertexArrayAttrib> attribs) {
  int index = 0;
  for (auto &attrib : attribs) {
    glBindAttribLocation(program.id, index, attrib.name);
    std::cout << attrib.name << " is " << index << "\n";
    glEnableVertexAttribArray(index);
    // clang-format off
    glVertexAttribPointer(index,
      ATTRIB_SPECS[(int)attrib.kind].size,
      ATTRIB_SPECS[(int)attrib.kind].type,
      attrib.normalized,
      struct_size, (void*) attrib.offset
    );
    // clang-format on
    index++;
  }
}

VertexArray::~VertexArray() {
  if (this->id != VertexArray::INVALID_ID) {
    glDeleteVertexArrays(1, &this->id);
  }
}

VertexArray::VertexArray(VertexArray &&other) noexcept {
  this->id = other.id;
  other.id = VertexArray::INVALID_ID;
}

VertexArray &VertexArray::operator=(VertexArray &&other) noexcept {
  if (this != &other) {
    this->id = other.id;
    other.id = VertexArray::INVALID_ID;
  }
  return *this;
}

void VertexArray::bind() {
  if (this->id != VertexArray::INVALID_ID)
    glBindVertexArray(this->id);
}

} // namespace RR
