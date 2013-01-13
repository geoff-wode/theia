/**
 Declaration of a Colour type - an RGBA 4-vector.
 */

#if ! defined( __THEIA_RENDERER_COLOUR__ )
#define __THEIA_RENDERER_COLOUR__

#include <glm/glm.hpp>

namespace theia
{
  struct Colour
  {
    Colour();
    Colour(float r, float g, float b, float a);
    Colour(const glm::vec4& v);
    Colour(const Colour& other);

    glm::vec4 v;

    static const Colour   TransparentBlack;
    static const Colour   Black;
    static const Colour   White;
    static const Colour   Red;
    static const Colour   Green;
    static const Colour   Blue;
  };

  bool operator==(const Colour& left, const Colour& right);
  bool operator!=(const Colour& left, const Colour& right);
}

#endif // __THEIA_RENDERER_COLOUR__
