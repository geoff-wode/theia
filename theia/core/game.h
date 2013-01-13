/**
 Declarations to glue the client code to the Core subsystem.
 */

#if ! defined(__THEIA_GAME__)
#define __THEIA_GAME__

#include <stdint.h>
#include <renderer/graphics_device.h>

/**
 Users must define the program's entry point.
 */
struct Program
{
  static int Run(int argc, char* argv[]);
};

namespace theia
{
  class Game
  {
  public:
    virtual ~Game();

    void Start();
    void Stop();

    // Called once following a call to Start to initialise the game instance.
    virtual void Initialise();

    // Called whenever a graphics context is been established.
    // This is the time to create any graphics objects (vertex buffers, etc) that
    // the game needs.
    virtual void DeviceReady();

    // Called when the graphics context is lost, e.g. after a screen resolution change.
    // At this point, any graphic objects (e.g. vertex buffers) are no longer available.
    virtual void DeviceLost();

    // Called as often as possible to enable the game to update its internal state.
    // elapsed is the number of milliseconds since the last call to Update.
    virtual void Update(uint32_t elapsed);

    // Called at ~60Hz.
    // The actual frequency will depend on the time taken to render the previous frame.
    // elapsed is the number of milliseconds since the last call to Render.
    virtual void Render(uint32_t elapsed);

    // Called when the game is stopping to allow proper clean-up.
    virtual void Stopping();

    // The Device is the factory for creating graphical objects e.g. vertex buffers.
    renderer::GraphicsDevice*  Device;

  protected:
    Game();
  };
}

#endif // __THEIA_GAME__
