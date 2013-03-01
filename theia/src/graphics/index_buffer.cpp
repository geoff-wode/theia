#include <theia/graphics/index_buffer.h>

using namespace theia;

IndexBufferPtr IndexBuffer::Create(size_t sizeInBytes)
{
  IndexBufferPtr vb(new IndexBuffer());

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vb->buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return vb;
}

IndexBuffer::IndexBuffer()
{
  glGenBuffers(1, &buffer);
}

IndexBuffer::~IndexBuffer()
{
  glDeleteBuffers(1, &buffer);
}

void IndexBuffer::SetData(size_t sizeInBytes, size_t offsetInBytes, const void* const data)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offsetInBytes, sizeInBytes, data);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
