#include <SDL.h>
#include <theia/graphics/device.h>
#include <theia/misc/debug.h>
#include "gl/loader.h"

using namespace theia;
using namespace theia::graphics;

//-------------------------------------------------------------------

class Device::Impl
{
public:
  Impl() 
  { }

  ~Impl()
  { }

  ClearContext  currentClearContext;
};

//-------------------------------------------------------------------

Device::Device()
  : FullScreen(false), Width(1280), Height(720),
    impl(new Device::Impl())
{
}

//-------------------------------------------------------------------
Device::~Device()
{
}

//-------------------------------------------------------------------

void Device::Initialise()
{
  struct Attribute
  {
    const char* name;
    SDL_GLattr  attr;
    int         value;
  };

  static const Attribute attributes[] =
  {
    { "SDL_GL_DOUBLEBUFFER",       SDL_GL_DOUBLEBUFFER,       1 },                            
    { "SDL_GL_RED_SIZE",           SDL_GL_RED_SIZE,           8 },                            
    { "SDL_GL_GREEN_SIZE",         SDL_GL_GREEN_SIZE,         8 },                            
    { "SDL_GL_BLUE_SIZE",          SDL_GL_BLUE_SIZE,          8 },                            
    { "SDL_GL_ALPHA_SIZE",         SDL_GL_ALPHA_SIZE,         8 },                            
    { "SDL_GL_BUFFER_SIZE",        SDL_GL_BUFFER_SIZE,        24 },                            
    { "SDL_GL_MULTISAMPLEBUFFERS", SDL_GL_MULTISAMPLEBUFFERS, 1 },                            
    { "SDL_GL_MULTISAMPLESAMPLES", SDL_GL_MULTISAMPLESAMPLES, 4 }
  };
  const int NumAttributes = sizeof(attributes)/sizeof(attributes[0]);

  for (int i = 0; i < NumAttributes; ++i)
  {
    SDL_GL_SetAttribute(attributes[i].attr, attributes[i].value);
  }

  int flags = SDL_OPENGL | SDL_HWSURFACE;
  if (FullScreen)
  {
    flags |= SDL_FULLSCREEN;
  }

  if (NULL == SDL_SetVideoMode(Width, Height, 32, flags))
  {
    LOG("device init failed - %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  ogl_LoadFunctions();
  LOG("GL v%d.%d loaded\n", ogl_GetMajorVersion(), ogl_GetMinorVersion());

  for (int i = 0; i < NumAttributes; ++i)
  {
    SDL_GL_SetAttribute(attributes[i].attr, attributes[i].value);

    int set;
    SDL_GL_GetAttribute(attributes[i].attr, &set);
    LOG("%s : %d --> %d\n", attributes[i].name, attributes[i].value, set);
  }

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
}

//-------------------------------------------------------------------

void Device::Clear(const ClearContext& context)
{
  if (context.Colour != impl->currentClearContext.Colour)
  {
    impl->currentClearContext.Colour = context.Colour;
    glClearColor(context.Colour.r, context.Colour.g, context.Colour.b, context.Colour.a);
  }

  if (context.Depth != impl->currentClearContext.Depth)
  {
    impl->currentClearContext.Depth = context.Depth;
    glClearDepth(context.Depth);
  }

  if (context.Stencil != impl->currentClearContext.Stencil)
  {
    impl->currentClearContext.Stencil = context.Stencil;
    glClearStencil(impl->currentClearContext.Stencil);
  }
    
  GLenum clearBufferBits = 0;
  if (ClearBuffer::Colour & context.BufferOptions)
  {
    clearBufferBits |= GL_COLOR_BUFFER_BIT;
  }
  if (ClearBuffer::DepthStencil & context.BufferOptions)
  {
    clearBufferBits |= GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
  }
  glClear(clearBufferBits);
}
