/// Implements the keyboard input handler.

#include <memory>
#include <SDL.h>
#include <theia/input/keyboard.h>

using namespace theia;
using namespace theia::input;

//--------------------------------------------------------------------

static Uint8 currentKeyState[SDLK_LAST];
static Uint8 previousKeyState[SDLK_LAST];

//--------------------------------------------------------------------

bool Keyboard::IsKeyDown(SDLKey key)
{
  return (1 == currentKeyState[key]);
}

bool Keyboard::IsKeyUp(SDLKey key)
{
  return (0 == currentKeyState[key]);
}

//--------------------------------------------------------------------

bool Keyboard::WasKeyDown(SDLKey key)
{
  return (1 == previousKeyState[key]);
}

bool Keyboard::WasKeyUp(SDLKey key)
{
  return (0 == previousKeyState[key]);
}

//--------------------------------------------------------------------

void Keyboard::Update()
{
  memcpy(previousKeyState, currentKeyState, sizeof(currentKeyState));
  memcpy(currentKeyState, SDL_GetKeyState(NULL), sizeof(currentKeyState));
}
