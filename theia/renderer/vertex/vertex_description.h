/// Defines per-vertex data.
/// Vertex data is composed of one or more vertex elements. Each vertex description
/// describes one "stream" of vertex data.

#if ! defined(__THEIA_VERTEX_DESCRIPTION__)
#define __THEIA_VERTEX_DESCRIPTION__

#include <vector>
#include <renderer/vertex/vertex_element.h>

namespace theia
{
  namespace renderer
  {
    struct VertexDescription
    {
      typedef std::vector<VertexElement>  ElementList;

      VertexDescription(const ElementList& elements);
      ~VertexDescription();

      /// Number of bytes from one vertex to the next.
      const uint32_t        Stride() const { return stride; }

      const uint32_t        ElementCount() const { return elementCount; }
      const VertexElement*  GetElements() const { return (const VertexElement*)elements; }

    private:
      uint32_t        stride;
      uint32_t        elementCount;
      VertexElement*  elements;
    };
  }
}

#endif // __THEIA_VERTEX_DESCRIPTION__
