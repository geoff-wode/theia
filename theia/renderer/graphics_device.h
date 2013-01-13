/**
 Low-level graphics device stuff.
 */

#if ! defined(__THEIA_GRAPHICS_DEVICE__)
#define __THEIA_GRAPHICS_DEVICE__

#include <stdint.h>
#include <glm/glm.hpp>

#include <renderer/colour.h>
#include <renderer/render_state/colour_mask.h>
#include <renderer/render_state/depth_stencil.h>

namespace theia
{
  namespace renderer
  {
    struct GraphicsDevice
    {
      GraphicsDevice();
      ~GraphicsDevice();

      bool      FullScreen;
      uint32_t  BackBufferWidth;
      uint32_t  BackBufferHeight;
      DepthStencilFormat::e DepthBufferFormat;

      void Initialise();

      void Clear(const Colour& colour);
    };
  }
}

#endif // __THEIA_GRAPHICS_DEVICE__
