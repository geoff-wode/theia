#include <SDL.h>
#include <gl_loader.h>

//----------------------------------------------

static void ConfigureSDL()
{
  static const struct SDLAttribute
  {
    const char* name;
    SDL_GLattr  attr;
    int         value;
  } sdlAttributes[] =
  {
    { "SDL_GL_DOUBLEBUFFER",  SDL_GL_DOUBLEBUFFER,            1 },
    { "SDL_GL_RED_SIZE",      SDL_GL_RED_SIZE,                8 },
    { "SDL_GL_GREEN_SIZE",    SDL_GL_GREEN_SIZE,              8 },
    { "SDL_GL_BLUE_SIZE",     SDL_GL_BLUE_SIZE,               8 },
    { "SDL_GL_ALPHA_SIZE",    SDL_GL_ALPHA_SIZE,              8 },
    { "SDL_GL_BUFFER_SIZE",   SDL_GL_BUFFER_SIZE,             24 },
    { "SDL_GL_MULTISAMPLEBUFFERS", SDL_GL_MULTISAMPLEBUFFERS, 1 },
    { "SDL_GL_MULTISAMPLESAMPLES", SDL_GL_MULTISAMPLESAMPLES, 4 }
  };
  const int NumAttributes = sizeof(sdlAttributes)/sizeof(sdlAttributes[0]);
  for (int i = 0; i < NumAttributes; ++i)
  {
    SDL_GL_SetAttribute(sdlAttributes[i].attr, sdlAttributes[i].value);
  }
}

//----------------------------------------------

static void ConfigureGL()
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);
}

//----------------------------------------------

void InitSystem()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  atexit(SDL_Quit);

  ConfigureSDL();
  if (NULL == SDL_SetVideoMode(800, 600, 32, SDL_OPENGL | SDL_HWSURFACE))
  {
    exit(EXIT_FAILURE);
  }
  ConfigureSDL();
  ogl_LoadFunctions();
  ConfigureGL();
}
