/**
 Entry point to the system.
 */

#include <stdio.h>
#include <stdlib.h>
#include <new>
#include <memory>
#include <SDL.h>
#include <core/game.h>
#include <renderer/graphics_device.h>

using namespace theia;
using namespace theia::core;
using namespace theia::renderer;

//----------------------------------------------------------

static bool quit = false;
static bool isActive = true;
static theia::core::Game* theGame = NULL;

//----------------------------------------------------------

static void HandleEvent(const SDL_Event& event);

// Update the internal state of the game.
static void UpdateGame(void);

// Process the next frame of game logic, using the elapsed time.
static void ProcessNextFrame(void);

static void OnActiveEvent(const SDL_ActiveEvent& event);
static void OnKeyboardEvent(const SDL_KeyboardEvent& event);
static void OnQuitEvent(const SDL_QuitEvent& event);

//----------------------------------------------------------

theia::core::Game::Game()
{
}

//----------------------------------------------------------

theia::core::Game::~Game()
{
}

//----------------------------------------------------------

void theia::core::Game::Initialise()
{
}

//----------------------------------------------------------

void theia::core::Game::LoadContent()
{
}

//----------------------------------------------------------

void theia::core::Game::UnloadContent()
{
}

//----------------------------------------------------------

void theia::core::Game::Update(uint32_t elapsed)
{
}

//----------------------------------------------------------

void theia::core::Game::Render(uint32_t elapsed)
{
  SDL_GL_SwapBuffers();
}

//----------------------------------------------------------

void theia::core::Game::Start()
{
  theGame = this;

  Initialise();
  LoadContent();

  while (!quit)
  {
    // Run until there is an event to be processed...
    while (!SDL_PollEvent(NULL))
    {
      UpdateGame();
    }

    // Process all events...
    SDL_Event event;
    while (!quit && SDL_PollEvent(&event))
    {
      HandleEvent(event);
    }
  }

  UnloadContent();
}

//----------------------------------------------------------

static void HandleEvent(const SDL_Event& event)
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

static void UpdateGame(void)
{
  static bool wasActive = false;

  if (isActive)
  {
    if (wasActive)
    {
      // Do one-time resuming of audio, etc...
      wasActive = false;
    }
    ProcessNextFrame();
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
}

//----------------------------------------------------------

static void OnQuitEvent(const SDL_QuitEvent& event)
{
  quit = true;
}

//----------------------------------------------------------

static void ProcessNextFrame(void)
{
  static Uint32 now = 0;
  static uint32_t lastUpdate = 0;
  static uint32_t lastRender = 0;

  now = SDL_GetTicks();
  uint32_t updateDelta = now - lastUpdate;
  uint32_t renderDelta = now - lastRender;

  // Run the update logic at a variable rate...
  theGame->Update(updateDelta);
  lastUpdate = now;

  // Run the render logic only at the screen's refresh rate (aiming for 60Hz)...
  const uint32_t ScreenRefreshRate = 1000 / 60;
  if (renderDelta > ScreenRefreshRate)
  {
    theGame->Render(renderDelta);
    lastRender = now;
  }
}
