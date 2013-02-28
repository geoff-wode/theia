#include <theia/graphics/vertex_buffer.h>

using namespace theia;

VertexBufferPtr VertexBuffer::Create(size_t sizeInBytes)
{
  VertexBufferPtr vb(new VertexBuffer());

  glBindBuffer(GL_ARRAY_BUFFER, vb->buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeInBytes, NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return vb;
}

VertexBuffer::VertexBuffer()
{
  glGenBuffers(1, &buffer);
}

VertexBuffer::~VertexBuffer()
{
  glDeleteBuffers(1, &buffer);
}

void VertexBuffer::SetData(size_t sizeInBytes, size_t offsetInBytes, const void* const data)
{
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferSubData(GL_ARRAY_BUFFER, sizeInBytes, offsetInBytes, data);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
