
#include <renderer/colour.h>

using namespace theia;

//----------------------------------------------------------

Colour::Colour()
  : v(0)
{
}

Colour::Colour(float r, float g, float b, float a)
  : v(r, g, b, a)
{
}

Colour::Colour(const glm::vec4& colour)
  : v(colour)
{
}

Colour::Colour(const Colour& other)
  : v(other.v)
{
}

//----------------------------------------------------------

bool theia::operator==(const Colour& left, const Colour& right)
{
  return
    (left.v.r == right.v.r) &&
    (left.v.g == right.v.g) &&
    (left.v.b == right.v.b) &&
    (left.v.a == right.v.a);
}

bool theia::operator!=(const Colour& left, const Colour& right)
{
  return ! (left == right);
}

//----------------------------------------------------------

const Colour Colour::TransparentBlack(0, 0, 0, 0);
const Colour Colour::Black(0, 0, 0, 1);
const Colour Colour::White(1, 1, 1, 1);
const Colour Colour::Red(1, 0, 0, 1);
const Colour Colour::Green(0, 1, 0, 1);
const Colour Colour::Blue(0, 0, 1, 1);
