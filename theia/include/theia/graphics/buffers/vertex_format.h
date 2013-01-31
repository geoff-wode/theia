/// A vertex format is a collection of vertex elements and an associated @see VertexBuffer
/// which contains data in that format.

#if ! defined(__THEIA_GFX_VERTEX_FORMAT__)
#define __THEIA_GFX_VERTEX_FORMAT__

#include <theia/graphics/buffers/vertex_element.h>
#include <theia/graphics/buffers/vertex_buffer.h>

namespace theia
{
  namespace graphics
  {
    class VertexFormat
    {
    public:
      /// Create a new vertex format which is described by the layout of the given elements.
      ///
      /// @param[in] elements   A set of @see VertexElement items which defines the layout of a
      ///                       vertex's data.
      VertexFormat(const VertexElementList& elements);

      /// Associate a @see VertexBuffer with this vertex format. This is an <i>association</i>; the
      /// VertexFormat object does not copy or take owenership of the buffer in question.
      ///
      /// @param[in] buffer     A @see VertexBuffer which contains vertex data.
      void SetBuffer(const VertexBuffer* const buffer);

      /// Associate multiple vertex buffers with this vertex format. This is an <i>association</i>; the
      /// VertexFormat object does not copy or take owenership of the buffer(s) in question.
      ///
      /// @param[in] buffers    An array of @see VertexBuffer pointers which contain vertex data.
      void SetBuffers(size_t count, const VertexBuffer* buffers[]);

      /// Returns a pointer to the @see VertexBuffer currently associated with this vertex format.
      VertexBuffer* const GetBuffer() const;

    private:
      const VertexElementList& elements;
      VertexBuffer* vb;
    };
  }
}

#endif // __THEIA_GFX_VERTEX_FORMAT__
