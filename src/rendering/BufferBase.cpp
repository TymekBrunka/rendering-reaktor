#include "BufferBase.hpp"
#include <iostream>

namespace RR {

BufferBase::BufferBase() { this->id = BufferBase::INVALID_ID; }

BufferBase::BufferBase(GLenum type) {
  glGenBuffers(1, &this->id);
  glBindBuffer(type, this->id);
}

BufferBase::BufferBase(GLenum type, const void *data[], GLsizeiptr amountOfBytes, GLenum usage) {
  glGenBuffers(1, &this->id);
  glBindBuffer(type, this->id);
  glBufferData(type, amountOfBytes, data, usage);
}

BufferBase::~BufferBase() {
  // std::cout << "Jam moge nie byc\n";
  if (this->id != BufferBase::INVALID_ID) {
    // std::cout << "Jam nie moc byc (usuwam" << this->id << ")\n";
    glDeleteBuffers(1, &this->id);
    // std::cout << "Jam usunal sie\n";
  }
  // std::cout << "Jam nie byc\n";
}

BufferBase::BufferBase(BufferBase &&other) noexcept {
  this->id = other.id;
  other.id = BufferBase::INVALID_ID;
}

BufferBase &BufferBase::operator=(BufferBase &&other) noexcept {
  if (this != &other) {
    this->id = other.id;
    other.id = BufferBase::INVALID_ID;
  }
  return *this;
}

void BufferBase::bind(GLenum type) {
  if (this->id != BufferBase::INVALID_ID)
    glBindBuffer(type, this->id);
}

} // namespace RR
