/**
 Encapsulation of a colour mask, enabling control over each RGBA component.
 */

#if ! defined(__THEIA_RENDERER_COLOUR_MASK__)
#define __THEIA_RENDERER_COLOUR_MASK__

namespace theia
{
  namespace renderer
  {
    struct ColourMask
    {
      ColourMask();
      ColourMask(bool r, bool g, bool b, bool a);
      ColourMask(const ColourMask& other);

      bool r;
      bool g;
      bool b;
      bool a;
    };
  
    bool operator==(const ColourMask& left, const ColourMask& right);
    bool operator!=(const ColourMask& left, const ColourMask& right);
  }
}

#endif // __THEIA_RENDERER_COLOUR_MASK__
