
#if ! defined(__THEIA_GFX_DEVICE__)
#define __THEIA_GFX_DEVICE__

#include <stdint.h>
#include <boost/scoped_ptr.hpp>
#include <glm/glm.hpp>
#include <theia/graphics/clear_context.h>

namespace theia
{
  namespace graphics
  {
    /// The device through which graphics are controlled and rendered.
    class Device
    {
    public:
      Device();
      ~Device();

      /// Called during application startup to initialise the graphics device.
      void Initialise();

      /// Clear the device using the given context.
      void Clear(const ClearContext& context);

      bool FullScreen;
      int Width;
      int Height;

    private:
      class Impl;
      boost::scoped_ptr<Impl> impl;
    };
  }
}

#endif // __THEIA_GFX_DEVICE__
