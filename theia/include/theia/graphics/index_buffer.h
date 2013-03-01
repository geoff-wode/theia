#if ! defined(__INDEX_BUFFER__)
#define __INDEX_BUFFER__

#include <boost/shared_ptr.hpp>
#include <theia/graphics/gl/gl_loader.h>

namespace theia
{
  struct IndexBuffer;
  typedef boost::shared_ptr<IndexBuffer> IndexBufferPtr;

  struct IndexBuffer
  {
    GLuint buffer;
    
    static IndexBufferPtr Create(size_t sizeInBytes);

    IndexBuffer();
    ~IndexBuffer();

    void SetData(size_t sizeInBytes, size_t offsetInBytes, const void* const data);
  };
}

#endif // __INDEX_BUFFER__
