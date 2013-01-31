
#include <vector>
#include "../gl_loader.h"
#include <theia/graphics/buffers/vertex_buffer.h>

using namespace theia;
using namespace theia::graphics;

//--------------------------------------------------------------------------------------

class VertexBufferImpl : public VertexBuffer
{
public:
  VertexBufferImpl(const VertexElementList& elements);
  virtual ~VertexBufferImpl();
  virtual void SetData(const void* const data, size_t count, size_t start);
  virtual void GetData(void* const data, size_t count, size_t start);
  virtual size_t GetVertexStride() const;
  virtual size_t GetVertexCount() const;

  virtual const VertexElementList& GetElements() const;

  GLuint buffer;
  size_t numVertices;
  size_t vertexStride;
  const VertexElementList& elements;
};

//--------------------------------------------------------------------------------------

VertexBuffer* VertexBuffer::Create(const VertexElementList& elements, size_t numVertices)
{
  VertexBufferImpl* vb = new VertexBufferImpl(elements);

  vb->numVertices = numVertices;

  // Compute the stride from the individual elements...
  vb->vertexStride = 0;
  for (size_t i = 0; i < vb->elements.size(); ++i)
  {
    vb->vertexStride += VertexElementType::Size(elements[i].Type());
  }

  const size_t sizeInBytes = numVertices * vb->vertexStride;

  glBindBuffer(GL_ARRAY_BUFFER, vb->buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeInBytes, NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return vb;
}

//--------------------------------------------------------------------------------------

VertexBuffer::VertexBuffer()
{
}

//--------------------------------------------------------------------------------------

VertexBuffer::~VertexBuffer()
{
}

//--------------------------------------------------------------------------------------

VertexBufferImpl::VertexBufferImpl(const VertexElementList& elements)
  : elements(elements)
{
  glGenBuffers(1, &buffer);
}

//--------------------------------------------------------------------------------------
VertexBufferImpl::~VertexBufferImpl()
{
  glDeleteBuffers(1, &buffer);
}

//--------------------------------------------------------------------------------------
void VertexBufferImpl::SetData(const void* const data, size_t count, size_t start)
{
  // Compute the byte offset into the buffer and the byte length of the intended copy...
  const size_t copyLength = count * vertexStride;
  const size_t copyStart = start * vertexStride;

  // Make sure the copy does not extend beyond the end of the buffer...
  const size_t bufferSize = vertexStride * numVertices;
  if ((copyStart + copyLength) < bufferSize)
  {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferSubData(GL_ARRAY_BUFFER, copyStart, copyLength, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  else
  {
    throw std::range_error("buffer overrun in VertexBuffer::SetData");
  }
}

//--------------------------------------------------------------------------------------
void VertexBufferImpl::GetData(void* const data, size_t count, size_t start)
{
  // Compute the byte offset into the buffer and the byte length of the intended copy...
  const size_t copyLength = count * vertexStride;
  const size_t copyStart = start * vertexStride;

  // Make sure the copy does not extend beyond the end of the buffer...
  const size_t bufferSize = vertexStride * numVertices;
  if ((copyStart + copyLength) < bufferSize)
  {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glGetBufferSubData(GL_ARRAY_BUFFER, copyStart, copyLength, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  else
  {
    throw std::range_error("buffer overrun in VertexBuffer::GetData");
  }
}

//--------------------------------------------------------------------------------------
size_t VertexBufferImpl::GetVertexStride() const
{
  return vertexStride;
}

//--------------------------------------------------------------------------------------
size_t VertexBufferImpl::GetVertexCount() const
{
  return numVertices;
}

//--------------------------------------------------------------------------------------
const VertexElementList& VertexBufferImpl::GetElements() const
{
  return elements;
}
