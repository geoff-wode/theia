/**
 Index buffer declarations.
 */


#include <renderer/gl/gl_loader.h>
#include <renderer/graphics_device.h>
#include <renderer/index_buffer.h>

using namespace theia;
using namespace theia::renderer;

//------------------------------------------------------------

IndexBuffer::IndexBuffer(const uint16_t indices[], uint16_t count)
  : Type(IndexBufferType::Short)
{
  glGenBuffers(1, &name);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, name);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint16_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(const uint32_t indices[], uint32_t count)
  : Type(IndexBufferType::Int)
{
  glGenBuffers(1, &name);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, name);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
  glDeleteBuffers(1, &name);
}

