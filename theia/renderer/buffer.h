/// Generic graphics buffer object.

#if ! defined(__THEIA_RENDERER_BUFFER__)
#define __THEIA_RENDERER_BUFFER__

#include <stdint.h>

namespace theia
{
  namespace renderer
  {
    namespace BufferType
    {
      enum e
      {
        VertexBuffer,
        IndexBuffer
      };
    }

    namespace BufferUsage
    {
      enum e
      {
        Static,         //< Buffer content never changes after creation.
        Dynamic,        //< Buffer content changes but is never read back to CPU.
        Bidirectional   //< Buffer content changes and/or is read and written to by CPU.
      };
    }

    struct Buffer
    {
      Buffer(BufferType::e type, BufferUsage::e usage);

      Buffer(BufferType::e type, BufferUsage::e usage, const uint8_t* const data, uint32_t size);

      virtual ~Buffer();

    private:
      const int     Type;
      const int     Usage;
      unsigned int  name;
    };
  }
}

#endif // __THEIA_RENDERER_BUFFER__
