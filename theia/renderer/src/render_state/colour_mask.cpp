#include <renderer/render_state/colour_mask.h>

using namespace theia;
using namespace theia::renderer;

//---------------------------------------------------------------

ColourMask::ColourMask()
  : r(true), g(true), b(true), a(true)
{
}

ColourMask::ColourMask(bool r, bool g, bool b, bool a)
  : r(r), g(g), b(b), a(a)
{
}

ColourMask::ColourMask(const ColourMask& other)
  : r(other.r), g(other.g), b(other.b), a(other.a)
{
}

//---------------------------------------------------------------

bool renderer::operator==(const ColourMask& left, const ColourMask& right)
{
  return (&left == &right);
}

bool renderer::operator!=(const ColourMask& left, const ColourMask& right)
{
  return ! (&left == &right);
}
