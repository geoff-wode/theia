
#if ! defined(__THEIA_GFX_DEVICE__)
#define __THEIA_GFX_DEVICE__

#include <stddef.h>
#include <theia/graphics/clear_state.h>

namespace theia
{
  namespace graphics
  {
    /// The basic device through which graphics are controlled and rendered.
    /// The device gives access to such things as the application window, the graphics renderer,
    /// the ability to create graphical resource like shaders and buffers, etc. etc. etc.
    class Device
    {
    public:
      Device();
      ~Device();

      /// Called during application startup to initialise the graphics device.
      void Initialise();

      // Get/set the width of the backbuffer. Default = 1280
      void SetBackbufferWidth(size_t value);
      size_t GetBackbufferWidth() const;

      // Get/set the height of the backbuffer. Default = 720.
      void SetBackbufferHeight(size_t value);
      size_t GetBackbufferHeight() const;

      /// Clears the backbuffer using the specified state. Actual state changes within the graphics
      /// driver are minimised.
      ///
      /// @param[in] clearState   The state settings that will be used for the clear operation.
      void Clear(const ClearState& clearState);

    private:
      class Impl;
      Impl* impl;
    };
  }
}

#endif // __THEIA_GFX_DEVICE__
