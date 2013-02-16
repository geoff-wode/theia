
#include <gl_loader.h>
#include <index_buffer.h>

//--------------------------------------------------------------------------------

struct IndexBuffer::Impl
{
  Impl() { glGenBuffers(1, &buffer); }
  ~Impl() { glDeleteBuffers(1, &buffer); }

  GLuint buffer;
  size_t numIndices;
  IndexBufferDataType::Enum dataType;
};

//--------------------------------------------------------------------------------
IndexBufferPtr IndexBuffer::Factory::New()
{
  IndexBufferPtr vb(new IndexBuffer());
  return vb;
}

//--------------------------------------------------------------------------------
IndexBuffer::IndexBuffer()
  : impl(new Impl())
{
}

//--------------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{
}

//--------------------------------------------------------------------------------
void IndexBuffer::Configure(size_t numIndices, IndexBufferDataType::Enum dataType)
{
  impl->numIndices = numIndices;
  impl->dataType = dataType;

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, impl->buffer);

  if (dataType == IndexBufferDataType::UnsignedInt)
  {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), NULL, GL_STATIC_DRAW);
  }
  else
  {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned short), NULL, GL_STATIC_DRAW);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//--------------------------------------------------------------------------------
size_t IndexBuffer::GetNumIndices() const
{
  return impl->numIndices;
}

//--------------------------------------------------------------------------------
IndexBufferDataType::Enum IndexBuffer::GetDataType() const
{
  return impl->dataType;
}

//--------------------------------------------------------------------------------
void IndexBuffer::SetData(size_t numIndices, size_t offset, const void* data)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, impl->buffer);

  if (impl->dataType == IndexBufferDataType::UnsignedInt)
  {
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(unsigned int), numIndices * sizeof(unsigned int), data);
  }
  else
  {
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(unsigned short), numIndices * sizeof(unsigned short), data);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//--------------------------------------------------------------------------------
void IndexBuffer::Apply() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, impl->buffer);
}
