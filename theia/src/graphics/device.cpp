#include <SDL.h>
#include <theia/graphics/device.h>
#include <theia/misc/debug.h>

#include "gl_loader.h"

using namespace theia;
using namespace theia::graphics;

//-------------------------------------------------------------------

static void ForceClearState(const ClearState& state);

//-------------------------------------------------------------------

class Device::Impl
{
public:
  Impl()
    : width(1280), height(720)
  {
  }

  size_t width;
  size_t height;
  ClearState currentClearState;
};

//-------------------------------------------------------------------

Device::Device()
{
  impl = new Device::Impl();
}

//-------------------------------------------------------------------

Device::~Device()
{
  delete impl;
}

//-------------------------------------------------------------------

void Device::Initialise()
{
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

  if (NULL == SDL_SetVideoMode(impl->width, impl->height, 32, SDL_OPENGL | SDL_HWSURFACE))
  {
    LOG("device init failed - %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  ogl_LoadFunctions();
  LOG("GL v%d.%d loaded\n", ogl_GetMajorVersion(), ogl_GetMinorVersion());

  ForceClearState(impl->currentClearState);
}

//-------------------------------------------------------------------

void Device::Clear(const ClearState& clearState)
{
  if (clearState.buffers != impl->currentClearState.buffers)
  {
    impl->currentClearState.buffers = clearState.buffers;
  }

  if (clearState.colour != impl->currentClearState.colour)
  {
    impl->currentClearState.colour = clearState.colour;
    glClearColor(clearState.colour.r, clearState.colour.g, clearState.colour.b, clearState.colour.a);
  }

  if (clearState.depth != impl->currentClearState.depth)
  {
    impl->currentClearState.depth = clearState.depth;
    glClearDepthf(impl->currentClearState.depth);
  }

  if (clearState.stencil != impl->currentClearState.stencil)
  {
    impl->currentClearState.stencil = clearState.stencil;
    glClearStencil(impl->currentClearState.stencil);
  }

  GLenum buffers = 0;
  if (clearState.buffers & BufferType::Colour) { buffers |= GL_COLOR_BUFFER_BIT; }
  if (clearState.buffers & BufferType::Colour) { buffers |= GL_DEPTH_BUFFER_BIT; }
  if (clearState.buffers & BufferType::Colour) { buffers |= GL_STENCIL_BUFFER_BIT; }
  glClear(buffers);
}

//-------------------------------------------------------------------

void Device::SetBackbufferWidth(size_t value)
{
  impl->width = value;
}

size_t Device::GetBackbufferWidth() const
{
  return impl->width;
}

//-------------------------------------------------------------------

void Device::SetBackbufferHeight(size_t value)
{
  impl->height = value;
}

size_t Device::GetBackbufferHeight() const
{
  return impl->height;
}

//-------------------------------------------------------------------
static void ForceClearState(const ClearState& state)
{
  glClearColor(state.colour.r, state.colour.g, state.colour.b, state.colour.a);
  glClearDepthf(state.depth);
  glClearStencil(state.stencil);
}
