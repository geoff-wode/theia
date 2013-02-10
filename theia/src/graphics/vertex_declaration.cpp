#include <memory>
#include <glm/glm.hpp>
#include "gl/loader.h"
#include <theia/graphics/vertex_declaration.h>

using namespace theia;
using namespace theia::graphics;

//-----------------------------------------------------------------------------

class VertexDeclaration::Impl
{
public:
  Impl(size_t numElements, const VertexElement* const elements)
    : vertexStride(ComputeStride(numElements, elements)),
      numElements(numElements),
      elements(elements)
  { }

  size_t  vertexStride;
  size_t  numElements;
  const VertexElement* const elements;

  size_t SizeofElementType(VertexElementType::Enum type)
  {
    switch (type)
    {
    case VertexElementType::Float:   return sizeof(float);
    case VertexElementType::Vector2: return sizeof(glm::vec2);
    case VertexElementType::Vector3: return sizeof(glm::vec3);
    case VertexElementType::Vector4: return sizeof(glm::vec4);
    default:
      break;
    }
    return 0;
  }

  size_t ComputeStride(size_t numElements, const VertexElement* const elements)
  {
    size_t stride = 0;
    for (size_t i = 0; i < numElements; ++i)
    {
      stride += SizeofElementType(elements[i].Type);
    }
    return stride;
  }
};

//-----------------------------------------------------------------------------

VertexDeclaration::VertexDeclaration(size_t numElements, const VertexElement* const elements)
  : impl(new VertexDeclaration::Impl(numElements, elements))
{
}

//-----------------------------------------------------------------------------
VertexDeclaration::~VertexDeclaration()
{
}

//-----------------------------------------------------------------------------

size_t VertexDeclaration::GetNumElements() const
{
  return impl->numElements;
}

//-----------------------------------------------------------------------------

const VertexElement* const VertexDeclaration::GetElements() const
{
  return impl->elements;
}

//-----------------------------------------------------------------------------

size_t VertexDeclaration::GetVertexStride() const
{
  return impl->vertexStride;
}
