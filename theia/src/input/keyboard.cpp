/// Implements the keyboard input handler.

#include <memory>
#include <SDL.h>
#include <theia/input/keyboard.h>

using namespace theia;
using namespace theia::input;

//--------------------------------------------------------------------
Keyboard::Keyboard()
{
}

//--------------------------------------------------------------------
Keyboard::~Keyboard()
{
}

//--------------------------------------------------------------------
bool Keyboard::IsKeyDown(SDLKey key) const
{
  const Uint8* keys = SDL_GetKeyState(NULL);
  return (1 == keys[key]);
}

//--------------------------------------------------------------------
bool Keyboard::IsKeyUp(SDLKey key) const
{
  const Uint8* keys = SDL_GetKeyState(NULL);
  return (0 == keys[key]);
}
