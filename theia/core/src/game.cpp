/**
 The Game class is the core object that drives all user interaction with the system.
 The application is kick-started by invoking the user's implementation of Program::Run
 which, in turn, must instantiate the user's game implementation and call Start on it.
 */

#include <stdio.h>
#include <stdlib.h>
#include <new>
#include <memory>
#include <SDL.h>
#include <core/game.h>
#include <renderer/graphics_device.h>
#include <core/keyboard.h>
#include <core/include/keyboard_impl.h>

using namespace theia;
using namespace theia::renderer;

//----------------------------------------------------------

static bool quit = false;
static bool isActive = true;
static theia::Game* theGame = NULL;

//----------------------------------------------------------

static void HandleEvent(const SDL_Event& event, Game* const game);

// Update the internal state of the game.
static void UpdateGame(Game* const game);

// Process the next frame of game logic, using the elapsed time.
static void ProcessNextFrame(Game* const game);

static void OnActiveEvent(const SDL_ActiveEvent& event);
static void OnKeyboardEvent(const SDL_KeyboardEvent& event);
static void OnQuitEvent(const SDL_QuitEvent& event);

//----------------------------------------------------------

theia::Game::Game()
{
}

//----------------------------------------------------------

theia::Game::~Game()
{
}

//----------------------------------------------------------

void theia::Game::Initialise()
{
}

//----------------------------------------------------------

void theia::Game::DeviceReady()
{
}

//----------------------------------------------------------

void theia::Game::DeviceLost()
{
}

//----------------------------------------------------------

void theia::Game::Stopping()
{
}

//----------------------------------------------------------

void theia::Game::Update(uint32_t elapsed)
{
}

//----------------------------------------------------------

void theia::Game::Render(uint32_t elapsed)
{
}

//----------------------------------------------------------

void theia::Game::Start()
{
  Initialise();
  DeviceReady();

  while (!quit)
  {
    // Run until there is an event to be processed...
    while (!SDL_PollEvent(NULL))
    {
      UpdateGame(this);
    }

    // Process all events...
    SDL_Event event;
    while (!quit && SDL_PollEvent(&event))
    {
      HandleEvent(event, this);
    }
  }

  DeviceLost();
}

//----------------------------------------------------------

void Game::Stop()
{
  quit = true;
}

//----------------------------------------------------------

static void HandleEvent(const SDL_Event& event, Game* const game)
{
  switch (event.type)
  {
  case SDL_ACTIVEEVENT:
    OnActiveEvent(event.active);
    break;
  case SDL_KEYDOWN:
  case SDL_KEYUP:
    OnKeyboardEvent(event.key);
    break;
  case SDL_QUIT:
    OnQuitEvent(event.quit);
    break;
  default:
    break;
  }
}

//----------------------------------------------------------

static void UpdateGame(Game* const game)
{
  static bool wasActive = false;

  if (isActive)
  {
    if (wasActive)
    {
      // Do one-time resuming of audio, etc...
      wasActive = false;
    }
    ProcessNextFrame(game);
    SDL_Delay(2);
  }
  else
  {
    if (!wasActive)
    {
      // Do one-time suspension of audio, etc.
      wasActive = true;
    }
  }
}

//----------------------------------------------------------

static void OnActiveEvent(const SDL_ActiveEvent& event)
{
  if (event.state & SDL_APPACTIVE)
  {
    if (0 == event.gain)
    {
      isActive = true;
    }
    else
    {
      isActive = false;
    }
  }
}

//----------------------------------------------------------

static void OnKeyboardEvent(const SDL_KeyboardEvent& event)
{
  if (SDL_KEYDOWN == event.type)
  {
    Keyboard::KeyWasPressed(event.keysym.sym);
  }
  else
  {
    Keyboard::KeyWasReleased(event.keysym.sym);
  }
}

//----------------------------------------------------------

static void OnQuitEvent(const SDL_QuitEvent& event)
{
  theGame->Stop();
}

//----------------------------------------------------------

static void ProcessNextFrame(Game* const game)
{
  static Uint32 now = 0;
  static uint32_t lastUpdate = 0;
  static uint32_t lastRender = 0;

  now = SDL_GetTicks();
  uint32_t updateDelta = now - lastUpdate;
  uint32_t renderDelta = now - lastRender;

  // Run the update logic at a variable rate...
  game->Update(updateDelta);
  lastUpdate = now;

  // Run the render logic only at the screen's refresh rate (aiming for 60Hz)...
  const uint32_t ScreenRefreshRate = 1000 / 60;
  if (renderDelta > ScreenRefreshRate)
  {
    game->Render(renderDelta);
    lastRender = now;

    SDL_GL_SwapBuffers();
  }
}
