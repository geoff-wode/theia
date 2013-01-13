/// Defines the data for one element in a vertex that will be passed into the rendering pipeline.

#if ! defined(_THEIA_VERTEX_ELEMENT__)
#define _THEIA_VERTEX_ELEMENT__

#include <stdint.h>

namespace theia
{
  namespace renderer
  {
    namespace VertexElementFormat
    {
      enum e
      {
        Float,
        Vector2,
        Vector3,
        Vector4,
        Half,
        HalfVector2,
        HalfVector3,
        HalfVector4,
        Colour,
        ByteVector4
      };
    }

    /// Define what the vertex element will be used for. This determines the bind point of the element
    /// data within the pipeline.
    namespace VertexElementUsage
    {
      enum e
      {
        Position,
        Colour,
        Normal
      };
    }

    struct VertexElement
    {
      VertexElement()
        : Offset(0), Format(VertexElementFormat::Float), Usage(VertexElementUsage::Position)
      {
      }

      VertexElement(uint32_t offset, VertexElementFormat::e format, VertexElementUsage::e usage)
        : Offset(offset), Format(format), Usage(usage)
      {
      }

      const uint32_t                Offset; //< Bytes from beginning of data to start of vertex data.
      const VertexElementFormat::e  Format;
      const VertexElementUsage::e   Usage;
    };
  }
}

#endif // _THEIA_VERTEX_ELEMENT__
