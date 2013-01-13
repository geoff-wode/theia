/**
 Entry point to the whole sorry affair, as called by SDL at startup.
 Does some basic warming up of the valves, then dives straight into the user's
 pathetic excuse for a program.
 */

#include <stdlib.h>
#include <SDL.h>
#include <core/game.h>
#include <core/include/keyboard_impl.h>

//----------------------------------------------------------

static void AtExit(void)
{
  SDL_Quit();
}

//----------------------------------------------------------

int main(int argc, char* argv[])
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    fprintf(stderr, "initialisation failed: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  atexit(AtExit);

  theia::Keyboard::Initialise();

  Program::Run(argc, argv);

  return 0;
}
