/// A vertex format is a collection of vertex elements and an associated @see VertexBuffer
/// which contains data in that format.

#if ! defined(__THEIA_GFX_VERTEX_DECLARATION__)
#define __THEIA_GFX_VERTEX_DECLARATION__

#include <theia/graphics/buffers/vertex_element.h>
#include <theia/graphics/buffers/vertex_buffer.h>

namespace theia
{
  namespace graphics
  {
    /// Declare a list of vertex elements.
    typedef std::vector<VertexElement>  VertexElementList;

    class VertexDeclaration
    {
    public:
      /// Create a new vertex format which is described by the layout of the given elements.
      /// The data associated with each vertex is assumed to be tightly packed in memory.
      ///
      /// @param[in] elements   A set of @see VertexElement items which defines the layout of a
      ///                       vertex's data.
      VertexDeclaration(const VertexElementList& elements);

      const VertexElementList& Elements() const { return elements; }

      size_t Stride() const { return stride; }

    private:
      size_t stride;
      VertexElementList elements;
    };
  }
}

#endif // __THEIA_GFX_VERTEX_DECLARATION__
