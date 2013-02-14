
#include <mesh.h>
#include <gl_loader.h>
#include <vertex_buffer.h>

struct Mesh::Impl
{
  ShaderPtr shader;
  VertexBufferPtr vertexBuffer;
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

  glDrawArrays(GL_TRIANGLES, 0, 3 * impl->vertexBuffer->GetNumVertices());
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
