
#include <theia/graphics/clear_context.h>

using namespace theia;
using namespace theia::graphics;

//-------------------------------------------------------------------

ClearContext::ClearContext()
  : BufferOptions(ClearBuffer::Colour), Colour(0), Depth(1.0f), Stencil(0)
{
}

ClearContext::~ClearContext()
{
}

//-------------------------------------------------------------------
