/**
 Delcare a vertex buffer which contains static data.
 Static vertex buffers are intended for data which is created once and never modified thereafter.
 @see DynamicVertexBuffer provides better performance for on-the-fly buffer updates.
 */

#if ! defined( __THEIA_VERTEX_BUFFER__ )
#define __THEIA_VERTEX_BUFFER__

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <stddef.h>
#include <theia/graphics/vertex_declaration.h>

namespace theia
{
  namespace graphics
  {
    class VertexBuffer;
    typedef boost::shared_ptr<VertexBuffer> VertexBufferPtr;

    class VertexBuffer
    {
    public:
      static VertexBufferPtr Create(size_t vertexCount, const VertexDeclaration& vertexDecl);

      ~VertexBuffer();

      void SetData(size_t vertexCount, size_t offset, const void* const data);

      void Enable() const;
      void Disable() const;

    private:
      VertexBuffer(size_t vertexCount, const VertexDeclaration& vertexDecl);
      class Impl;
      boost::scoped_ptr<Impl> impl;
    };
  }
}

#endif // __THEIA_VERTEX_BUFFER__
