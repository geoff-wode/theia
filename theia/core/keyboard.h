/**
 Interface to the system-level keyboard handler.
 */

#if ! defined( __THEIA_CORE_KEYBOARD__ )
#define __THEIA_CORE_KEYBOARD__

#include <vector>
#include <SDL.h>

namespace theia
{
  namespace Keyboard
  {
    bool IsKeyUp(SDLKey key);
    bool IsKeyDown(SDLKey key);
  };
}

#endif // __THEIA_CORE_KEYBOARD__
