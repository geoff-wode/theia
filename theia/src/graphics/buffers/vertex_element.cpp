
#include <glm/glm.hpp>
#include <glm/gtc/half_float.hpp>
#include <theia/graphics/buffers/vertex_element.h>
#include "../gl_loader.h"

using namespace theia;
using namespace theia::graphics;

//-----------------------------------------------------------------

// A structure containing various interesting implementation details about vertex elements.
struct ElementUsageInfo
{
  const char* name;
  VertexElementType::Enum type;
};

struct ElementTypeInfo
{
  const char* name;
  size_t  numComponents;
  size_t  sizeInBytes;
  GLenum  glType;
};

//-----------------------------------------------------------------

static ElementUsageInfo  elementUsageInfo[] =
{
  { "Position", VertexElementType::Vector3 },
  { "Normal",   VertexElementType::Vector3 },
  { "Colour",   VertexElementType::Vector4 },
  { "TexCoord", VertexElementType::Vector2 }
};

static ElementTypeInfo elementTypeInfo[] =
{
  { "Float",        1, sizeof(float),       GL_FLOAT },
  { "Vector2",      2, sizeof(glm::vec2),   GL_FLOAT },
  { "Vector3",      3, sizeof(glm::vec3),   GL_FLOAT },
  { "Vector4",      4, sizeof(glm::vec4),   GL_FLOAT },
  { "HalfVector2",  2, sizeof(glm::hvec4),  GL_FLOAT },
  { "HalfVector4",  4, sizeof(glm::hvec4),  GL_FLOAT },
  { "Colour",       4, sizeof(glm::vec4),   GL_FLOAT }
};

//-----------------------------------------------------------------

VertexElement::VertexElement(VertexElementType::Enum type,
                             VertexElementUsage::Enum usage, unsigned int usageIndex,
                             size_t offset, unsigned int stream)
  : Type(type),
    Size(elementTypeInfo[(int)Type].sizeInBytes),
    Usage(usage), UsageIndex(usageIndex),
    Offset(offset),
    Stream(stream)
{
  if (Offset > 2047)
  {
    throw std::range_error("vertex element offset too large");
  }
  if (stream > 15)
  {
    throw std::range_error("vertex element stream index too large");
  }
}

//-----------------------------------------------------------------
VertexElement::VertexElement(const VertexElement& other)
  : Type(other.Type),
    Size(other.Size),
    Usage(other.Usage), UsageIndex(other.UsageIndex),
    Offset(other.Offset),
    Stream(other.Stream)
{
}

//-----------------------------------------------------------------
const char* const theia::graphics::VertexElementUsage::ToString(VertexElementUsage::Enum usage)
{
  return elementUsageInfo[(int)usage].name;
}

const char* const theia::graphics::VertexElementType::ToString(VertexElementType::Enum type)
{
  return elementTypeInfo[(int)type].name;
}
