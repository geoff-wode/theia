


#include <theia/graphics/vertex_element.h>

using namespace theia;
using namespace theia::graphics;

//-----------------------------------------------------------------------------

VertexElement::VertexElement(VertexElementType::Enum type, VertexElementUsage::Enum usage, unsigned int index, size_t offset)
  : Type(type), Usage(usage), Index(index), Offset(offset)
{
}
