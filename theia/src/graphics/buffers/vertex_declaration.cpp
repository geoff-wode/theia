/// Implements the type of vertex attribute/data split described in
/// http://www.opengl.org/wiki/Vertex_Specification#Separate_attribute_format.

#include <stdlib.h>
#include "../gl_loader.h"
#include <theia/graphics/buffers/vertex_declaration.h>

using namespace theia;
using namespace theia::graphics;

//--------------------------------------------------------------------------------------

VertexDeclaration::VertexDeclaration(const VertexElementList& elements)
  : stride(0), elements(elements)
{
  // Compute the stride as the sum of the element sizes.
  for (size_t i = 0; i < elements.size(); ++i)
  {
    stride += elements[i].Size;
  }
}
