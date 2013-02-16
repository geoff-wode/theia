

#if ! defined(__MESH__)
#define __MESH__

#include <gl_loader.h>
#include <shader.h>
#include <vertex_buffer.h>
#include <index_buffer.h>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

class Mesh;
typedef boost::shared_ptr<Mesh> MeshPtr;

class Mesh
{
public:
  struct Factory
  {
    static MeshPtr New();
  };

  ~Mesh();

  void SetShader(ShaderPtr shader);
  void SetVertexBuffer(VertexBufferPtr vertexBuffer);
  void SetIndexBuffer(IndexBufferPtr indexBuffer);

  void Render() const;

private:
  Mesh();
  struct Impl;
  boost::scoped_ptr<Impl> impl;
};


#endif // __MESH__
