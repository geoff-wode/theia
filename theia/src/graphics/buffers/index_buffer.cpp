
#include <stdint.h>
#include "../gl/loader.h"
#include <theia/graphics/buffers/index_buffer.h>

using namespace theia;
using namespace theia::graphics;

//----------------------------------------------------------------------

class IndexBuffer::Impl
{
public:
  Impl() : buffer(0), sizeInBytes(0) { glGenBuffers(1, &buffer); }
  ~Impl() { glDeleteBuffers(1, &buffer); }

  GLuint buffer;
  size_t sizeInBytes;
  size_t typeSize;
};

//----------------------------------------------------------------------

IndexBufferPtr IndexBuffer::Create(size_t indexCount, IndexBufferDataType::Enum type)
{
  IndexBufferPtr ib(new IndexBuffer(indexCount, type));
  return ib;
}

//----------------------------------------------------------------------

IndexBuffer::IndexBuffer(size_t indexCount, IndexBufferDataType::Enum type)
  : impl(new IndexBuffer::Impl())
{
  if (IndexBufferDataType::UInt16 == type)
  {
    impl->typeSize = sizeof(uint16_t);
  }
  else
  {
    impl->typeSize = sizeof(uint32_t);
  }
  impl->sizeInBytes = indexCount * impl->typeSize;

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, impl->buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, impl->sizeInBytes, NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//----------------------------------------------------------------------

IndexBuffer::~IndexBuffer()
{
}

//----------------------------------------------------------------------

void IndexBuffer::SetData(size_t indexCount, size_t offset, const void* const data)
{
  const size_t copyStart = offset * impl->typeSize;
  const size_t copyLength = indexCount * impl->typeSize;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, impl->buffer);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, copyLength, copyStart, data);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


//----------------------------------------------------------------------

void IndexBuffer::Enable() const { glBindBuffer(GL_ARRAY_BUFFER, impl->buffer); }
void IndexBuffer::Disable() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
