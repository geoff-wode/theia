/// A vertex buffer is a list of vertices that is passed to the graphics card for rendering.

#if ! defined(__THEIA_GFX_VERTEX_BUFFER__)
#define __THEIA_GFX_VERTEX_BUFFER__

#include <vector>
#include <stddef.h>
#include <theia/graphics/buffers/vertex_element.h>

namespace theia
{
  namespace graphics
  {
    /// A vertex buffer for static data. Use this object for data which does not change over the course
    /// of the application's life. This object is not optimised for dynamic updates to vertex data and
    /// is likely to cause a performance hit if used in that way. For dynamic vertex data use @see
    /// DynamicVertexBuffer.
    class VertexBuffer
    {
    public:
      /// Create a new vertex buffer that will contain vertices in a format specified by an array of
      /// vertex elements.
      ///
      /// @note The data is assumed to be tightly packed into memory.
      ///
      /// @param[in] elements     An array of @see VertexElement which collectively define the vertex
      ///                         memory layout.
      /// @param[in] numElements  The number of elements in the array.
      /// @param[in] numVertices  The maximum number of vertices the buffer will contain.
      static VertexBuffer* Create(const VertexElementList& elements, size_t numVertices);

      virtual ~VertexBuffer() = 0;

      /// Copy data into the vertex buffer, specifying the number of vertices and start offset within
      /// the buffer. For this method, the buffer can be considered as an array of vertex structures
      /// which were defined during buffer construction.
      ///
      /// @note While not an error per se to call this method repeatedly, there is a likely to be a
      /// performance hit. Use @see DynamicVertexBuffer instead.
      ///
      /// @param[in] data     The user data to copy into the buffer. This must match the definition provided
      ///                     when the buffer was created.
      /// @param[in] count    The number of vertices to copy.
      /// @param[in] start    The starting index within the buffer at which copying should begin.
      virtual void SetData(const void* const data, size_t count, size_t start) = 0;

      /// Copy data from the vertex buffer into user memory, specifying the number of vertices and start offset
      /// within the buffer. For this method, the buffer can be considered as an array of vertex structures
      /// which were defined during buffer construction.
      ///
      /// @note While not an error per se to call this method, there is a likely to be a performance hit. Use
      /// @see DynamicVertexBuffer instead.
      ///
      /// @param[in] data     Pointer to where the buffer data will be copied. It is assumed that the buffer is
      ///                     large enough to accomodate @see count vertices.
      /// @param[in] count    The number of vertices to copy.
      /// @param[in] start    The starting index within the buffer from which copying should begin.
      virtual void GetData(void* const data, size_t count, size_t start) = 0;

      virtual size_t GetVertexStride() const = 0;

      virtual size_t GetVertexCount() const = 0;

      virtual const VertexElementList& GetElements() const = 0;

    protected:
      VertexBuffer();
    };
  }
}

#endif // __THEIA_GFX_VERTEX_BUFFER__
