#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <renderer/gl_loader.h>
#include <renderer/graphics_device.h>

using namespace theia;
using namespace theia::renderer;

//----------------------------------------------------------

static void OnExit(void);

//----------------------------------------------------------

Property<uint32_t> GraphicsDevice::BPP = 24;
Property<uint32_t> GraphicsDevice::Width = 800;
Property<uint32_t> GraphicsDevice::Height = 600;

//----------------------------------------------------------

void GraphicsDevice::Initialise()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    fprintf(stderr, "initialisation failed: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  atexit(OnExit);

  struct GLAttr
  {
    GLAttr(SDL_GLattr name, int value)
      : name(name), value(value)
    {
    }
    const SDL_GLattr name;
    const int value;
  };

  const GLAttr attrs[] =
  {
    GLAttr(SDL_GL_RED_SIZE, 8),
    GLAttr(SDL_GL_GREEN_SIZE, 8),
    GLAttr(SDL_GL_BLUE_SIZE, 8),
    GLAttr(SDL_GL_DEPTH_SIZE, 16),
    GLAttr(SDL_GL_DOUBLEBUFFER, 1)
  };
  const int NumAttrs = sizeof(attrs) / sizeof(attrs[0]);

  for (int i = 0; i < NumAttrs; ++i)
  {
    SDL_GL_SetAttribute(attrs[i].name, attrs[i].value);
  }

  if (SDL_SetVideoMode(GraphicsDevice::Width, GraphicsDevice::Height, GraphicsDevice::BPP, SDL_OPENGL) == NULL)
  {
    fprintf(stderr, "failed to set video mode: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  if (ogl_LOAD_FAILED == ogl_LoadFunctions())
  {
    fprintf(stderr, "failed to initialise gl\n");
    exit(EXIT_FAILURE);
  }
}

//----------------------------------------------------------

static void OnExit(void)
{
  SDL_Quit();
}
