#include <renderer/gl/gl_loader.h>
#include <renderer/vertex/vertex_buffer.h>

using namespace theia;
using namespace theia::renderer;

//-----------------------------------------------------------------------

VertexBuffer::VertexBuffer(const VertexDescription& desc, uint32_t count, VertexBufferUsage::e usage)
  : usage(usage), desc(desc)
{
  size = desc.Stride() * count;
  glGenBuffers(1, &name);
  glBindBuffer(GL_ARRAY_BUFFER, size);

  GLenum glUsage = 0;
  switch (usage)
  {
  case VertexBufferUsage::Static:
    glUsage = GL_STATIC_DRAW;
    break;
  case VertexBufferUsage::Dynamic:
    glUsage = GL_DYNAMIC_DRAW;
    break;
  case VertexBufferUsage::Transfer:
    glUsage = GL_DYNAMIC_COPY;
    break;
  default:
    break;
  };

  glBufferData(GL_ARRAY_BUFFER, size, NULL, glUsage);
}

VertexBuffer::~VertexBuffer()
{
  glDeleteBuffers(1, &name);
}

//-----------------------------------------------------------------------

void VertexBuffer::SetData(const void* const data, uint32_t offset, uint32_t count)
{
  const uint32_t byteOffset = offset * desc.Stride();
  const uint32_t byteCopySize = count * desc.Stride();
  const uint32_t byteLength = byteOffset + byteCopySize;

  if (byteLength < Size())
  {
    int32_t currentBuffer;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentBuffer);
    if (name != currentBuffer)
    {
      glBindBuffer(GL_ARRAY_BUFFER, name);
    }
    glBufferSubData(GL_ARRAY_BUFFER, byteOffset, byteCopySize, data);
  }
}

//-----------------------------------------------------------------------

void VertexBuffer::GetData(void* const data, uint32_t offset, uint32_t count)
{
  const uint32_t byteOffset = offset * desc.Stride();
  const uint32_t byteCopySize = count * desc.Stride();
  const uint32_t byteLength = byteOffset + byteCopySize;

  if (byteLength < Size())
  {
    int32_t currentBuffer;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentBuffer);
    if (name != currentBuffer)
    {
      glBindBuffer(GL_ARRAY_BUFFER, name);
    }
    glGetBufferSubData(GL_ARRAY_BUFFER, byteOffset, byteCopySize, data);
  }
}
