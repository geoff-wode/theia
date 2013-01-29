#include <theia/graphics/clear_state.h>

using namespace theia;
using namespace theia::graphics;

//---------------------------------------------------

ClearState::ClearState()
  : buffers(BufferType::Colour),
    colour(0),
    depth(1.0f),
    stencil(0)
{
}
