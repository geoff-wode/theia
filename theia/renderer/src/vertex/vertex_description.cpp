#include <glm/glm.hpp>
#include <glm/gtc/half_float.hpp>
#include <renderer/gl/gl_loader.h>
#include <renderer/vertex/vertex_description.h>

using namespace theia;
using namespace theia::renderer;

//-------------------------------------------------------------------

VertexDescription::VertexDescription(const ElementList& elements)
{
  this->elements = new VertexElement[elements.size()];
  stride = 0;
  for (unsigned int i = 0; i < elements.size(); ++i)
  {
    switch (elements[i].Format)
    {
      case VertexElementFormat::Float:
        stride += sizeof(float);
        break;
      case VertexElementFormat::Vector2:
        stride += sizeof(glm::vec2);
        break;
      case VertexElementFormat::Vector3:
        stride += sizeof(glm::vec3);
        break;
      case VertexElementFormat::Vector4:
        stride += sizeof(glm::vec4);
        break;
      case VertexElementFormat::Half:
        stride += sizeof(glm::half);
        break;
      case VertexElementFormat::HalfVector2:
        stride += sizeof(glm::hvec2);
        break;
      case VertexElementFormat::HalfVector3:
        stride += sizeof(glm::hvec3);
        break;
      case VertexElementFormat::HalfVector4:
        stride += sizeof(glm::hvec4);
        break;
      case VertexElementFormat::Colour:
        stride += sizeof(uint32_t);
        break;
      case VertexElementFormat::ByteVector4:
        stride += sizeof(glm::bvec4);
        break;
      default:
        break;
    }
    memcpy(&this->elements[i], &elements[i], sizeof(VertexElement));
  }
}

//-------------------------------------------------------------------

VertexDescription::~VertexDescription()
{
  delete [] elements;
}
