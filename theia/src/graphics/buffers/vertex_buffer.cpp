

#include "../gl/loader.h"
#include <theia/graphics/buffers/vertex_buffer.h>

using namespace theia;
using namespace theia::graphics;

//----------------------------------------------------------------------

class VertexBuffer::Impl
{
public:
  Impl()
    : buffer(0), sizeInBytes(0)
  {
  }
  ~Impl()
  {
    glDeleteBuffers(1, &buffer);
  }

  GLuint  buffer;
  size_t  sizeInBytes;
  const VertexDeclaration*  decl;
};

//----------------------------------------------------------------------

static size_t ElementComponents(VertexElementType::Enum type)
{
  switch (type)
  {
  case VertexElementType::Float:   return 1;
  case VertexElementType::Vector2: return 2;
  case VertexElementType::Vector3: return 3;
  case VertexElementType::Vector4: return 4;
  }
  return 0;
}

//----------------------------------------------------------------------

static GLenum ElementBaseType(VertexElementType::Enum type)
{
  switch (type)
  {
  case VertexElementType::Float:   return GL_FLOAT;
  case VertexElementType::Vector2: return GL_FLOAT;
  case VertexElementType::Vector3: return GL_FLOAT;
  case VertexElementType::Vector4: return GL_FLOAT;
  }
  return 0;
}

//----------------------------------------------------------------------

VertexBufferPtr VertexBuffer::Create(size_t vertexCount, const VertexDeclaration& vertexDecl)
{
  VertexBufferPtr vb(new VertexBuffer(vertexCount, vertexDecl));
  return vb;
}

//----------------------------------------------------------------------

VertexBuffer::VertexBuffer(size_t vertexCount, const VertexDeclaration& vertexDecl)
  : impl(new VertexBuffer::Impl())
{
  impl->decl = &vertexDecl;
  impl->sizeInBytes = vertexCount * vertexDecl.GetVertexStride();
  
  // Create the buffer object and allocate memory to it...
  glGenBuffers(1, &impl->buffer);
  glBindBuffer(GL_ARRAY_BUFFER, impl->buffer);
  glBufferData(GL_ARRAY_BUFFER, impl->sizeInBytes, NULL, GL_STATIC_DRAW);

  // Bind the vertex element definitions...
  const VertexElement* const elements = impl->decl->GetElements();
  for (size_t e = 0; e < impl->decl->GetNumElements(); ++e)
  {
    // The element usage and index combine to provide for multiple elements of the same usage.
    // E.g. there can be multiple elements with TextureCoordinate usage, which have indices
    // [0..16].
    unsigned int usage = elements[e].Usage + (16 * elements[e].Index);
    glEnableVertexAttribArray(usage);
    glVertexAttribPointer(
      usage,
      ElementComponents(elements[e].Type),
      ElementBaseType(elements[e].Type),
      GL_FALSE,   // Values are never normalised by the GL.
      impl->decl->GetVertexStride(),
      (const void*)elements[e].Offset);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//----------------------------------------------------------------------

VertexBuffer::~VertexBuffer()
{
}

//----------------------------------------------------------------------

void VertexBuffer::SetData(size_t vertexCount, size_t offset, const void* const data)
{
  const size_t copyStart = offset * impl->decl->GetVertexStride();
  const size_t copyLength = vertexCount * impl->decl->GetVertexStride();

  glBindBuffer(GL_ARRAY_BUFFER, impl->buffer);
  glBufferSubData(GL_ARRAY_BUFFER, copyLength, copyStart, data);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//----------------------------------------------------------------------

void VertexBuffer::Enable() const { glBindBuffer(GL_ARRAY_BUFFER, impl->buffer); }
void VertexBuffer::Disable() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
