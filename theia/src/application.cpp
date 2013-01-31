/// Defines the base class for all application objects.

#include <stddef.h>
#include <stdlib.h>
#include <SDL.h>
#include <theia/application.h>

using namespace theia;

//----------------------------------------------------------------------------

class Application::Impl
{
public:
  Impl(Application* app)
    : app(app), running(true)
  {
    app->Update(0);
  }

  void HandleEvents();

  Application* app;
  bool running;
};

//----------------------------------------------------------------------------

void Application::Impl::HandleEvents()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (SDL_QUIT == event.type)
    {
      running = false;
      return;
    }
    else
    {
    }
  }
}

//----------------------------------------------------------------------------

Application::Application()
  : impl(NULL)
{
}

//----------------------------------------------------------------------------

Application::~Application()
{
  delete impl;
}

//----------------------------------------------------------------------------

void Application::Start()
{
  impl = new Impl(this);

  SDL_Init(SDL_INIT_EVERYTHING);
  atexit(SDL_Quit);

  Device.Initialise();

  Initialise();

  unsigned int previousUpdateFrame = SDL_GetTicks();
  unsigned int previousRenderFrame = SDL_GetTicks();
  unsigned int currentFrame = 0;
  unsigned int elapsedMS = 0;

  // The main application loop.
  // Update and Render are called as often as possible with an indication as to how long ago (in milliseconds) it
  // was previously called. The elapsed time passed to Render takes account of the amount of time that was spent
  // in the immediately preceding Update call.
  do
  {
    Keyboard.Update();

    // Use the "time" now to work out how long ago the previous call to Update was...
    currentFrame = SDL_GetTicks();
    elapsedMS = currentFrame - previousUpdateFrame;
    Update(elapsedMS);
    previousUpdateFrame = currentFrame;

    // Use the "time" now to work out how long ago the previous call to Render was...
    currentFrame = SDL_GetTicks();
    elapsedMS = currentFrame - previousRenderFrame;
    Render(elapsedMS);
    previousRenderFrame = currentFrame;

    SDL_GL_SwapBuffers();
    SDL_Delay(2);

    impl->HandleEvents();
  } while (impl->running);

  // Tell the derived class that it needs to tidy up now...
  Exiting();
}

//----------------------------------------------------------------------------

void Application::Stop()
{
  // Do a thread-safe shutdown by posting a quit event into the event queue...
  SDL_Event event;
  event.type = SDL_QUIT;
  SDL_PushEvent(&event);
}

//----------------------------------------------------------------------------

void Application::Initialise()
{
}

//----------------------------------------------------------------------------

void Application::Exiting()
{
}

//----------------------------------------------------------------------------

void Application::Update(unsigned int elapsedMS)
{
}

//----------------------------------------------------------------------------

void Application::Render(unsigned int elapsedMS)
{
}

//----------------------------------------------------------------------------
