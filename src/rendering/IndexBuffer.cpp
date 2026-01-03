#include "IndexBuffer.hpp"
#include "BufferBase.hpp"

#include "impl_buffer_move.hpp"

namespace RR {

IndexBuffer::IndexBuffer(const GLuint data[], GLsizeiptr n, GLenum usage)
  : BufferBase(GL_ELEMENT_ARRAY_BUFFER, (const void **)data, n * sizeof(GLuint), usage)
{
  this->length = n;
}

__RR_impl_buffer_move(IndexBuffer, GL_ELEMENT_ARRAY_BUFFER, 
  this->length = other.length;
)

void IndexBuffer::bind() {
  BufferBase::bind(GL_ELEMENT_ARRAY_BUFFER);
}

} // namespace RR
