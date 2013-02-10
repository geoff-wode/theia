/**
  Declaration of a single vertex element (position, normal, etc.) in a vertex structure.
 */

#if ! defined (__THEIA_VERTEX_ELEMENT__)
#define __THEIA_VERTEX_ELEMENT__

#include <stddef.h>

namespace theia
{
  namespace graphics
  {
    /// Declares vertex element usage.
    namespace VertexElementUsage
    {
      enum Enum
      {
        Position,
        Normal,
        TextureCoord,
        Colour
      };
    }

    /// Declares the possible types of a vertex element.
    namespace VertexElementType
    {
      enum Enum
      {
        Float,
        Vector2,
        Vector3,
        Vector4
      };
    }

    struct VertexElement
    {
      VertexElement(VertexElementType::Enum type, VertexElementUsage::Enum usage, unsigned int index, size_t offset);

      const VertexElementType::Enum   Type;   //< Element type.
      const VertexElementUsage::Enum  Usage;  //< Element usage.
      const unsigned int              Index;  //< Element usage index.
      const size_t                    Offset; //< Byte offset from start of vertex structure.
    };
  }
}

#endif // __THEIA_VERTEX_ELEMENT__
