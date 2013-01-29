/// Defines the state settings which can be applied to a device clear.

#if ! defined(__THEIA_GFX_CLEAR_STATE__)
#define __THEIA_GFX_CLEAR_STATE__

#include <stdint.h>
#include <glm/glm.hpp>

namespace theia
{
  namespace graphics
  {
    namespace BufferType
    {
      enum Enum
      {
        Colour  = 1 << 0,
        Depth   = 1 << 1,
        Stencil = 1 << 2,
        DepthStencil = Depth | Stencil,
        All = Colour | Depth | Stencil
      };
    }

    class ClearState
    {
    public:
      /// Instantiates a clear state object with the default values.
      ClearState();

      /// The buffers to clear. Default = Colour.
      BufferType::Enum  buffers;

      /// The colour to which to set the colour buffer. Default = (0, 0, 0, 0).
      glm::vec4 colour;

      /// The value to which to set the depth buffer. Default = 1.0f.
      float depth;

      /// The value to which to set the stencil buffer. Default = 0.
      uint8_t stencil;
    };
  }
}

#endif // __THEIA_GFX_CLEAR_STATE__
