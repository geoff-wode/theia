#if ! defined(__MATERIAL__)
#define __MATERIAL__

#include <glm/glm.hpp>
#include <theia/graphics/shader.h>

namespace theia
{
  struct MaterialState
  {
    MaterialState(ShaderPtr shader);

    glm::vec3 Ke;
    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec4 Ks;

    ShaderPtr shader;
    Shader::Parameter* const emissiveParam;
    Shader::Parameter* const ambientParam;
    Shader::Parameter* const diffuseParam;
    Shader::Parameter* const specularParam;
  };

  namespace Material
  {
    /// Apply the current material settings.
    void Apply(const MaterialState& material);
  }
}

#endif // __MATERIAL__
