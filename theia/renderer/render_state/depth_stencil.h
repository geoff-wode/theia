/**
 Declarations for the depth-stencil buffer.

 Depth and stencil buffers are logically combined into a single buffer.
 to prevent stencil buffer creation, use a non-stencilled buffer format.
 */

#if ! defined(__THEIA_RENDERER_DEPTH_STENCIL__)
#define __THEIA_RENDERER_DEPTH_STENCIL__

namespace theia
{
  namespace renderer
  {
    namespace DepthStencilFormat
    {
      enum e
      {
        None,
        Depth16,
        Depth24Stencil8,
        Depth32
      };
    }
  }
}

#endif // __THEIA_RENDERER_DEPTH_STENCIL__
