/// Vertex buffers are untyped memory blocks containing vertex data.

#if ! defined(__THEIA_VERTEX_BUFFER__)
#define __THEIA_VERTEX_BUFFER__

#include <stdint.h>
#include <renderer/vertex/vertex_description.h>

namespace theia
{
  namespace renderer
  {
    /// Hint to the graphics driver about how the buffer will be used.
    namespace VertexBufferUsage
    {
      enum e
      {
        Static,     //< Once populated, the data is never modified or read back by the CPU.
        Dynamic,    //< Some or all of the data is modified by the CPU but not read back.
        Transfer    //< The data is read back by the CPU.
      };
    }

    struct VertexBuffer
    {
      /// Create a new buffer to store 'count' vertices which match the description.
      VertexBuffer(const VertexDescription& desc, uint32_t count, VertexBufferUsage::e usage);
      virtual ~VertexBuffer();

      /// Copy the data into the buffer. The amount of data to copy is computed from the
      /// requested count of vertices and the size of each vertex as defined in the
      /// description used to create the buffer.
      ///
      /// @param[in] data   user data to copy into the buffer
      /// @param[in] offset position, <i>in vertices</i> from start of buffer at which to copy data
      /// @param[in] count  number of vertices to copy
      void SetData(const void* const data, uint32_t offset, uint32_t count);

      /// Copy the content of the buffer out to user memory. The amount of data to copy
      /// is computed from the requested count of vertices and the size of each vertex
      /// as defined in the description used to create the buffer.
      ///
      /// @param[in] data   user data pointer to copy the buffer content to
      /// @param[in] offset position, <i>in vertices</i> from start of buffer from which to copy data
      /// @param[in] count  number of vertices to copy
      void GetData(void* const data, uint32_t offset, uint32_t count);

      void GetDescription(VertexDescription& desc) const;

      /// Size of the buffer in bytes.
      uint32_t  Size() const { return size; }
      VertexBufferUsage::e  Usage() const { return usage; }

    private:
      uint32_t  name;
      uint32_t  size;
      VertexBufferUsage::e  usage;
      VertexDescription     desc;
    };
  }
}

#endif // __THEIA_VERTEX_BUFFER__
