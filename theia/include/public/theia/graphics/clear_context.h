/**
 Defines the contextual state when calling @see Device::Clear.
 */

#if ! defined( __THEIA_CLEAR_CONTEXT__ )
#define __THEIA_CLEAR_CONTEXT__

#include <stdint.h>
#include <glm/glm.hpp>

namespace theia
{
  namespace graphics
  {
    namespace ClearBuffer
    {
      enum Enum
      {
        Colour = 1 << 0,
        DepthStencil = 1 << 1,
        All = Colour | DepthStencil
      };
    }

    class ClearContext
    {
    public:
      ClearContext();
      ~ClearContext();

      ClearBuffer::Enum   BufferOptions;  //< Default = colour buffer.
      glm::vec4           Colour;         //< Default = transparent black.
      float               Depth;          //< Default = 1.0f.
      uint8_t             Stencil;        //< Default = 0.
    };
  }
}

#endif // __THEIA_CLEAR_CONTEXT__
