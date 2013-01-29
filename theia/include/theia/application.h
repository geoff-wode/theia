
#if ! defined(__THEIA_APPLICTAION__)
#define __THEIA_APPLICTAION__

#include <theia/graphics/device.h>
#include <theia/input/keyboard.h>

namespace theia
{
  /// Main class containing interfaces to control the application.
  /// Users should derive from this class and override its methods to implement their applications.
  class Application
  {
  public:
    virtual ~Application();

    /// Causes the application object to be initialised and then to enter the update/render loop.
    void Start();

    /// Causes the update/render loop to exit and the application object to be destroyed.
    /// This method is thread-safe, allowing it to be called from anywhere in the system.
    void Stop();

    graphics::Device Device;
    input::Keyboard Keyboard;

  protected:
    /// Application objects can be created only by derivation.
    Application();

    /// Called when the application is started to enable it to perform one-time initialisation of
    /// internal state and allocate resources which will exist for the life-time of the application.
    virtual void Initialise();

    /// Called just before the application is terminated. This is where any remaining resources should
    /// be freed.
    virtual void Exiting();

    /// Allows the application to run its internal logic. The method is called as often as possible.
    ///
    /// @param[in] elapsedMS  The number of milliseconds which have elapsed since the previous call to Update.
    virtual void Update(unsigned int elapsedMS);

    /// Allows the application to render itself to the screen. The method is called as often as possible.
    ///
    /// @param[in] elapsedMS  The number of milliseconds which have elapsed since the previous call to Render.
    virtual void Render(unsigned int elapsedMS);

  private:
    /// Application objects cannot be copied.
    Application(const Application&);

    /// Application objects cannot be copied.
    Application& operator=(const Application);

    /// Internal object details.
    class Impl;
    Impl* impl;
  };
}

#endif // __THEIA_APPLICTAION__
