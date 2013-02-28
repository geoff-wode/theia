#if ! defined(__VERTEX_BUFFER__)
#define __VERTEX_BUFFER__

#include <boost/shared_ptr.hpp>
#include <theia/graphics/gl/gl_loader.h>

namespace theia
{
  struct VertexBuffer;
  typedef boost::shared_ptr<VertexBuffer> VertexBufferPtr;

  struct VertexBuffer
  {
    GLuint buffer;
    
    static VertexBufferPtr Create(size_t sizeInBytes);

    VertexBuffer();
    ~VertexBuffer();

    void SetData(size_t sizeInBytes, size_t offsetInBytes, const void* const data);
  };
}

#endif // __VERTEX_BUFFER__
