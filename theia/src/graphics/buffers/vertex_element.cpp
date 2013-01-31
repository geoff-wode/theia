
#include <glm/glm.hpp>
#include <glm/gtc/half_float.hpp>
#include <theia/graphics/buffers/vertex_element.h>

using namespace theia;
using namespace theia::graphics;

//-----------------------------------------------------------------

VertexElement::VertexElement(VertexElementType::Enum type, VertexElementUsage::Enum usage, unsigned int index, size_t offset)
  : type(type), usage(usage), index(index), offset(offset)
{
}

VertexElement::VertexElement(const VertexElement& other)
  : type(other.type), usage(other.usage), index(other.index)
{
}

//-----------------------------------------------------------------

size_t VertexElementType::Size(VertexElementType::Enum type)
{
  switch (type)
  {
  case Float      : return sizeof(float); break;
  case Vector2    : return sizeof(glm::vec2); break;
  case Vector3    : return sizeof(glm::vec3); break;
  case Vector4    : return sizeof(glm::vec4); break;
  case HalfVector2: return sizeof(glm::vec2); break;
  case HalfVector3: return sizeof(glm::hvec3); break;
  case HalfVector4: return sizeof(glm::hvec4); break;
  case Colour     : return sizeof(glm::vec4); break;
  default:
    break;
  }
  return 0;
}
