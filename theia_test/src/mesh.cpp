
#include <mesh.h>
#include <gl_loader.h>
#include <vertex_buffer.h>
#include <index_buffer.h>

struct Mesh::Impl
{
  Impl() : shader(NULL), vertexBuffer(NULL), indexBuffer(NULL) { }

  ShaderPtr shader;
  VertexBufferPtr vertexBuffer;
  IndexBufferPtr  indexBuffer;
};

//--------------------------------------------------------------------------------
Mesh::Mesh()
  : impl(new Impl())
{
}

//--------------------------------------------------------------------------------
Mesh::~Mesh()
{
}

//--------------------------------------------------------------------------------
MeshPtr Mesh::Factory::New()
{
  MeshPtr mesh(new Mesh());
  return mesh;
}

//--------------------------------------------------------------------------------
void Mesh::Render() const
{
  impl->shader->Apply();
  impl->vertexBuffer->Apply();
  impl->indexBuffer->Apply();

  if (impl->indexBuffer->GetDataType() == IndexBufferDataType::UnsignedInt)
  {
    glDrawElements(GL_TRIANGLES, impl->indexBuffer->GetNumIndices(), GL_UNSIGNED_INT, (const void*)0);
  }
  else
  {
    glDrawElements(GL_TRIANGLES, impl->indexBuffer->GetNumIndices(), GL_UNSIGNED_SHORT, (const void*)0);
  }
}

//--------------------------------------------------------------------------------
void Mesh::SetVertexBuffer(VertexBufferPtr vertexBuffer)
{
  impl->vertexBuffer = vertexBuffer;
}

//--------------------------------------------------------------------------------
void Mesh::SetShader(ShaderPtr shader)
{
  impl->shader = shader;
}

//--------------------------------------------------------------------------------
void Mesh::SetIndexBuffer(IndexBufferPtr indexBuffer)
{
  impl->indexBuffer = indexBuffer;
}
