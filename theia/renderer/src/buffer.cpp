
#include <renderer/gl/gl_loader.h>
#include <renderer/buffer.h>

using namespace theia;
using namespace theia::renderer;

//------------------------------------------------------------------------------------

static GLenum GLBufferType(BufferType::e type)
{
  switch (type)
  {
  case BufferType::VertexBuffer:
    return GL_ARRAY_BUFFER;
  case BufferType::IndexBuffer:
    return GL_ELEMENT_ARRAY_BUFFER;
  default:
    break;
  }
  return static_cast<GLenum>(0);
}

//------------------------------------------------------------------------------------
static GLenum GLBufferUsage(BufferUsage::e usage)
{
  switch (usage)
  {
  case BufferUsage::Static:
    return GL_STATIC_DRAW;
  case BufferUsage::Dynamic:
    return GL_DYNAMIC_DRAW;
  case BufferUsage::Bidirectional:
    return GL_DYNAMIC_COPY;
  default:
    break;
  }
  return static_cast<GLenum>(0);
}

//------------------------------------------------------------------------------------

Buffer::Buffer(BufferType::e type, BufferUsage::e usage)
  : Type(GLBufferType(type)), Usage(GLBufferUsage(usage))
{
  glGenBuffers(1, &name);
}

Buffer::Buffer(BufferType::e type, BufferUsage::e usage, const uint8_t* const data, uint32_t size)
  : Type(GLBufferType(type)), Usage(GLBufferUsage(usage))
{
  glGenBuffers(1, &name);
  glBindBuffer(Type, name);
  glBufferData(Type, size, data, Usage);
  glBindBuffer(Type, 0);
}

Buffer::~Buffer()
{
  glDeleteBuffers(1, &name);
}
