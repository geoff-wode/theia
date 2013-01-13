/**
 Essentially a factory for creating renderer objects.
 */

#include <stdlib.h>
#include <new>
#include <SDL.h>
#include <glm/glm.hpp>
#include <renderer/gl/gl_loader.h>
#include <renderer/graphics_device.h>
#include <renderer/render_state/depth_stencil.h>

using namespace theia;
using namespace theia::renderer;

//-----------------------------------------------------

static void AtExit(void);
static bool InitGL(GraphicsDevice* device);

static void ApplyColourMask(const ColourMask& mask);
static void ApplyDepthMask(bool mask);

//-----------------------------------------------------

//-----------------------------------------------------

GraphicsDevice::GraphicsDevice()
  : FullScreen(false),
    BackBufferWidth(800), BackBufferHeight(600),
    DepthBufferFormat(DepthStencilFormat::None)
{
}

//-----------------------------------------------------
GraphicsDevice::~GraphicsDevice()
{
}

//-----------------------------------------------------

void GraphicsDevice::Initialise()
{
  if (!InitGL(this))
  {
    exit(EXIT_FAILURE);
  }
}

//-----------------------------------------------------

void GraphicsDevice::Clear(const Colour& colour)
{
  int bitMask = 0;

  //if (currentClearState.buffers.Colour) { bitMask |= GL_COLOR_BUFFER_BIT; }
  //if (currentClearState.buffers.Depth) { bitMask |= GL_DEPTH_BUFFER_BIT; }
  //if (currentClearState.buffers.Stencil) { bitMask |= GL_STENCIL_BUFFER_BIT; }

  glClear(bitMask);
}

//-----------------------------------------------------

static bool InitGL(GraphicsDevice* device)
{
  bool initialised = false;

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
//  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  int fullscreen = (device->FullScreen ? SDL_FULLSCREEN : 0);
  if (SDL_SetVideoMode(device->BackBufferWidth, device->BackBufferHeight, 24, SDL_OPENGL | fullscreen) != NULL)
  {
    if (ogl_LOAD_FAILED != ogl_LoadFunctions())
    {
      initialised = true;
    }
  }
  else
  {
    fprintf(stderr, "failed to set video mode: %s\n", SDL_GetError());
  }

  return initialised;
}

//-----------------------------------------------------

static void ApplyColourMask(const ColourMask& mask)
{
  //if (mask != currentClearState.colourMask)
  //{
  //  currentClearState.colourMask = mask;
  //  glColorMask(
  //    currentClearState.colourMask.r,
  //    currentClearState.colourMask.g,
  //    currentClearState.colourMask.b,
  //    currentClearState.colourMask.a);
  //}
}

//-----------------------------------------------------

static void ApplyDepthMask(bool mask)
{
  //if (mask != currentClearState.depthMask)
  //{
  //  currentClearState.depthMask = mask;
  //  glDepthMask(mask ? GL_TRUE : GL_FALSE);
  //}
}
