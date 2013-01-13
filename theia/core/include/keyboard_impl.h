/**
 Implementation of low level keyboard handling.
 */

#if ! defined( __THEIA_CORE_KEYBOARD_IMPL__ )
#define __THEIA_CORE_KEYBOARD_IMPL__

#include <SDL.h>

namespace theia
{
  namespace Keyboard
  {
    void Initialise();
    void KeyWasPressed(SDLKey key);
    void KeyWasReleased(SDLKey key);
  }
}

#endif // __THEIA_CORE_KEYBOARD_IMPL__
