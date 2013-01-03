/**
 Low-level graphics device stuff.
 */

#if ! defined(__THEIA_GRAPHICS_DEVICE__)
#define __THEIA_GRAPHICS_DEVICE__

#include <stdint.h>
#include <utils/property.h>

namespace theia
{
  namespace renderer
  {
    struct GraphicsDevice
    {
      static void Initialise();

      static Property<uint32_t> Width;
      static Property<uint32_t> Height;
      static Property<uint32_t> BPP;
    };
  }
}

#endif // __THEIA_GRAPHICS_DEVICE__
