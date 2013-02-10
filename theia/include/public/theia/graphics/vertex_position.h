/**
 Declares a vertex data type containing just a 3D position vector.
 */

#if ! defined( __THEIA_VERTEX_POSITION__ )
#define __THEIA_VERTEX_POSITION__

#include <glm/glm.hpp>
#include <theia/graphics/vertex_declaration.h>

namespace theia
{
  namespace graphics
  {
    struct VertexPosition
    {
      VertexPosition() : pos(0) { }
      VertexPosition(const glm::vec3& p) : pos(p) { }

      glm::vec3 pos;

      static const VertexDeclaration& Declaration;
    };
  }
}

#endif // __THEIA_VERTEX_POSITION__
