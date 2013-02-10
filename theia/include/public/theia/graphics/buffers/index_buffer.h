/**
 Delcare an index buffer which contains static data.
 Static index buffers are intended for data which is created once and never modified thereafter.
 @see DynamicIndexBuffer provides better performance for on-the-fly buffer updates.
 */

#if ! defined( __THEIA_INDEX_BUFFER__ )
#define __THEIA_INDEX_BUFFER__

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <stddef.h>

namespace theia
{
  namespace graphics
  {
    class IndexBuffer;
    typedef boost::shared_ptr<IndexBuffer> IndexBufferPtr;

    /// Determines the type of index value stored in the buffer.
    namespace IndexBufferDataType
    {
      enum Enum
      {
        UInt16,  //< The buffer's data is composed of 16-bit unsigned values.
        UInt32   //< The buffer's data is composed of 32-bit unsigned values.
      };
    }

    class IndexBuffer
    {
    public:
      /// Create an index buffer large enough to hold indexCount indices of the given type.
      static IndexBufferPtr Create(size_t indexCount, IndexBufferDataType::Enum type);

      ~IndexBuffer();

      /// Copy indexCount index values from the memory at data into the buffer starting at
      /// the given offset.
      /// @note The offset is specified as a number of indices <b>not</b> a number of bytes.
      void SetData(size_t indexCount, size_t offset, const void* const data);

      void Enable() const;
      void Disable() const;

    private:
      IndexBuffer(size_t indexCount, IndexBufferDataType::Enum type);
      class Impl;
      boost::scoped_ptr<Impl> impl;
    };
  }
}

#endif // __THEIA_INDEX_BUFFER__
