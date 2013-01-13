#if ! defined(__THEIA_RENDERER_CLEAR_BUFFERS__)
#define __THEIA_RENDERER_CLEAR_BUFFERS__

namespace theia
{
  namespace renderer
  {
    struct ClearBuffers
    {
      ClearBuffers();

      bool Colour;
      bool Depth;
      bool Stencil;
    };
  }
}

#endif // __THEIA_RENDERER_CLEAR_BUFFERS__
