
#include <gl_loader.h>
#include <vertex_decl.h>
#include <vertex_buffer.h>

//--------------------------------------------------------------------------------

struct VertexBuffer::Impl
{
  Impl() { glGenBuffers(1, &buffer); }
  ~Impl() { glDeleteBuffers(1, &buffer); }
  GLuint buffer;
  size_t numVertices;
  const VertexDeclaration* vertexDecl;
};

//--------------------------------------------------------------------------------
VertexBufferPtr VertexBuffer::Factory::New()
{
  VertexBufferPtr vb(new VertexBuffer());
  return vb;
}

//--------------------------------------------------------------------------------
VertexBuffer::VertexBuffer()
  : impl(new Impl())
{
}

//--------------------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{
}

//--------------------------------------------------------------------------------
void VertexBuffer::Configure(const VertexDeclaration* const vertexDecl, size_t numVertices)
{
  impl->numVertices = numVertices;
  impl->vertexDecl = vertexDecl;

  glBindBuffer(GL_ARRAY_BUFFER, impl->buffer);
  glBufferData(GL_ARRAY_BUFFER, numVertices * vertexDecl->stride, NULL, GL_STATIC_DRAW);

  for (size_t i = 0; i < vertexDecl->numElements; ++i)
  {
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(
      i,
      vertexDecl->elements[i].components,
      vertexDecl->elements[i].type,
      GL_FALSE,
      vertexDecl->stride,
      (const void*)vertexDecl->elements[i].offset);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//--------------------------------------------------------------------------------
const VertexDeclaration* const VertexBuffer::GetDeclaration() const
{
  return impl->vertexDecl;
}

//--------------------------------------------------------------------------------
size_t VertexBuffer::GetNumVertices() const
{
  return impl->numVertices;
}

//--------------------------------------------------------------------------------
void VertexBuffer::SetData(size_t numVertices, size_t offset, const void* data)
{
  glBindBuffer(GL_ARRAY_BUFFER, impl->buffer);
  glBufferSubData(GL_ARRAY_BUFFER, offset, numVertices * impl->vertexDecl->stride, data);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//--------------------------------------------------------------------------------
void VertexBuffer::Apply() const
{
  glBindBuffer(GL_ARRAY_BUFFER, impl->buffer);
}
