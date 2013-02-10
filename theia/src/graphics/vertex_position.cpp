#include <stddef.h>
#include <theia/graphics/vertex_declaration.h>
#include <theia/graphics/vertex_position.h>

using namespace theia;
using namespace theia::graphics;

//---------------------------------------------------------

static const VertexElement elements[] =
{
  VertexElement(VertexElementType::Vector3, VertexElementUsage::Position, 0, offsetof(VertexPosition, pos))
};

static const VertexDeclaration vd(1, elements);
const VertexDeclaration& VertexPosition::Declaration = vd;
