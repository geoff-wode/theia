#if ! defined(__VERTEX_BUFFER__)
#define __VERTEX_BUFFER__

#include <gl_loader.h>
#include <vertex_decl.h>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

class VertexBuffer;
typedef boost::shared_ptr<VertexBuffer> VertexBufferPtr;

class VertexBuffer
{
public:
  struct Factory
  {
    static VertexBufferPtr New();
  };

  ~VertexBuffer();

  void Configure(const VertexDeclaration* const vertexDecl, size_t numVertices);

  size_t GetNumVertices() const;
  const VertexDeclaration* const GetDeclaration() const;

  void SetData(size_t numVertices, size_t offset, const void* data);

  void Apply() const;

private:
  VertexBuffer();
  struct Impl;
  boost::scoped_ptr<Impl> impl;
};

#endif // __VERTEX_BUFFE__
