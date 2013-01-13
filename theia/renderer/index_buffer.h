/**
 Index buffer declarations.
 */

#if ! defined(__THEIA_RENDERER_INDEX_BUFFER__)
#define __THEIA_RENDERER_INDEX_BUFFER__

#include <stdint.h>

namespace theia
{
  namespace renderer
  {
    namespace IndexBufferType
    {
      enum e
      {
        Short,
        Int
      };
    }

    struct IndexBuffer
    {
      explicit IndexBuffer(const uint16_t indices[], uint16_t count);
      explicit IndexBuffer(const uint32_t indices[], uint32_t count);

      ~IndexBuffer();

      const IndexBufferType::e  Type;

      uint32_t  name;
    };
  }
}

#endif // __THEIA_RENDERER_INDEX_BUFFER__
