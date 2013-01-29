/// Declares the object that provides access to keyboard input.

#if ! defined(__THEIA_INPUT_KEYBOARD__)
#define __THEIA_INPUT_KEYBOARD__

#include <vector>
#include <SDL_keyboard.h>

namespace theia
{
  namespace input
  {
    class Keyboard
    {
    public:
      Keyboard();
      virtual ~Keyboard();

      virtual bool IsKeyDown(SDLKey key) const;
      virtual bool IsKeyUp(SDLKey key) const;
    };
  }
}

#endif // __THEIA_INPUT_KEYBOARD__
