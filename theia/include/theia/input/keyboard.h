/// Declares the object that provides access to keyboard input.

#if ! defined(__THEIA_INPUT_KEYBOARD__)
#define __THEIA_INPUT_KEYBOARD__

#include <SDL_keyboard.h>

namespace theia
{
  namespace input
  {
    class Keyboard
    {
    public:
      bool IsKeyDown(SDLKey key);
      bool IsKeyUp(SDLKey key);
      bool WasKeyDown(SDLKey key);
      bool WasKeyUp(SDLKey key);

      /// Called by the Application base class.
      void Update();
    };
  }
}

#endif // __THEIA_INPUT_KEYBOARD__
