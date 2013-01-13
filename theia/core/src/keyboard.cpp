/**
 Implementation of low level keyboard handling.
 */

#include <stdlib.h>
#include <SDL.h>
#include <core/keyboard.h>
#include <core/include/keyboard_impl.h>

using namespace theia;

//-------------------------------------------------------------------

static bool keyState[SDLK_LAST] = { false };

//-------------------------------------------------------------------

bool Keyboard::IsKeyUp(SDLKey key)
{
  return !keyState[key];
}

//-------------------------------------------------------------------

bool Keyboard::IsKeyDown(SDLKey key)
{
  return keyState[key];
}

//-------------------------------------------------------------------

void Keyboard::Initialise()
{
}

//-------------------------------------------------------------------

void Keyboard::KeyWasPressed(SDLKey key)
{
  keyState[key] = true;
}

//-------------------------------------------------------------------

void Keyboard::KeyWasReleased(SDLKey key)
{
  keyState[key] = false;
}
