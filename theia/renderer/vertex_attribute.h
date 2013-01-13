/**
 Declare an object that describes the in-memory layout of a vertex.
 */

#if ! defined(__THEIA_RENDERER_VERTEX_ATTR__)
#define __THEIA_RENDERER_VERTEX_ATTR__

#include <stdint.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>

namespace theia
{
  namespace renderer
  {
    namespace VertexAttrDataType
    {
      enum e
      {
        Int,
        UInt,
        Float,
        Half,
        Vector2,
        Vector3,
        Vector4,
        Matrix
      };
    }

    /**
      A vertex attribute is one peice of data embedded in a vertex structure.
      E.g. an interleaved vertex buffer with a position and normal at each vertex could be

        struct VertexPositionNormal
        {
          glm::vec3   Position;
          glm::hvec3  Normal;
        };

        static const VertexAttribute VertexPositionNormal[] =
        {
          { VertexAttrDataType::Float, 3, 0, sizeof(float) * 6 },
          { VertexAttrDataType::Half, 3, sizeof(float) * 3, sizeof(float) * 6 }
        }
      */
    struct VertexAttribute
    {
      VertexAttribute(VertexAttrDataType::e type, uint8_t size, uint32_t offset, uint32_t stride);

      const VertexAttrDataType::e Type;
      const uint8_t               Size;     // number of components in this attribute
      const uint32_t              Offset;   // offset to attribute from start of buffer
      const uint32_t              Stride;   // distance to next attribute
    };
  }
}

#endif // __THEIA_RENDERER_VERTEX_ATTR__
