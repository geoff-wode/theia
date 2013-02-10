/**
 A structure defining per-vertex data.
 */

#if ! defined( __THEIA_VERTEX_DECLARATION__ )
#define __THEIA_VERTEX_DECLARATION__

#include <boost/scoped_ptr.hpp>
#include <stddef.h>
#include <theia/graphics/vertex_element.h>

namespace theia
{
  namespace graphics
  {
    class VertexDeclaration
    {
    public:
      VertexDeclaration(size_t numElements, const VertexElement* const elements);
      ~VertexDeclaration();

      size_t  GetVertexStride() const;  //< Number of bytes from one vertex to the next.

      size_t  GetNumElements() const;
      const VertexElement* const  GetElements() const;

    private:
      class Impl;
      boost::scoped_ptr<Impl> impl;
    };
  }
}

#endif // __THEIA_VERTEX_DECLARATION__
