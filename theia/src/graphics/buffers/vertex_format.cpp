/// Implements the type of vertex attribute/data split described in
/// http://www.opengl.org/wiki/Vertex_Specification#Separate_attribute_format.

#include <stdlib.h>
#include "../gl_loader.h"
#include <theia/graphics/buffers/vertex_format.h>

using namespace theia;
using namespace theia::graphics;

//--------------------------------------------------------------------------------------
VertexFormat::VertexFormat(const VertexElementList& elements)
  : elements(elements), vb(NULL)
{
}

//--------------------------------------------------------------------------------------
void VertexFormat::SetBuffer(const VertexBuffer* const buffer)
{
  vb = (VertexBuffer*)buffer;
}

//--------------------------------------------------------------------------------------
VertexBuffer* const VertexFormat::GetBuffer() const
{
  return vb;
}
