
#include <vector>
#include "../gl_loader.h"
#include <theia/graphics/buffers/vertex_buffer.h>

using namespace theia;
using namespace theia::graphics;

//--------------------------------------------------------------------------------------

class VertexBufferImpl : public VertexBuffer
{
public:
  VertexBufferImpl();
  virtual ~VertexBufferImpl();
  virtual void SetData(const void* const data, size_t count, size_t start);
  virtual void GetData(void* const data, size_t count, size_t start);
  virtual size_t GetVertexCount() const;
  virtual const VertexDeclaration& GetVertexDeclaration() const;
  virtual void Enable() const;
  virtual void Disable() const;

  GLuint buffer;
  size_t numVertices;
  VertexDeclaration vertexDecl;
};

//--------------------------------------------------------------------------------------

VertexBuffer* VertexBuffer::Create(const VertexDeclaration& vertexDecl, size_t numVertices)
{
  VertexBufferImpl* vb = new VertexBufferImpl();

  vb->numVertices = numVertices;
  vb->vertexDecl = vertexDecl;

  const size_t sizeInBytes = numVertices * vertexDecl.Stride();

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

VertexBufferImpl::VertexBufferImpl()
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
  const size_t copyLength = count * vertexDecl.Stride();
  const size_t copyStart = start * vertexDecl.Stride();

  // Make sure the copy does not extend beyond the end of the buffer...
  const size_t bufferSize = vertexDecl.Stride() * numVertices;
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
  const size_t copyLength = count * vertexDecl.Stride();
  const size_t copyStart = start * vertexDecl.Stride();

  // Make sure the copy does not extend beyond the end of the buffer...
  const size_t bufferSize = vertexDecl.Stride() * numVertices;
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
size_t VertexBufferImpl::GetVertexCount() const
{
  return numVertices;
}

//--------------------------------------------------------------------------------------
const VertexDeclaration& VertexBufferImpl::GetVertexDeclaration() const
{
  return vertexDecl;
}

//--------------------------------------------------------------------------------------

void VertexBufferImpl::Enable() const
{
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  for (size_t i = 0; i < vertexDecl.Elements().size(); ++i)
  {
    const GLuint shaderAttrLocation = ((unsigned int)vertexDecl.Elements()[i].Usage * 16) + vertexDecl.Elements()[i].UsageIndex;

    glVertexAttribFormat(
      shaderAttrLocation,
      elementTypeInfo[(int)Type].numComponents,
      elementTypeInfo[(int)Type].glType,
      GL_FALSE, // never let GL normalise the values - always make the user do it explicitly
      Offset);
  }
}

//--------------------------------------------------------------------------------------

void VertexBufferImpl::Disable() const
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
