#if ! defined(__MATERIAL__)
#define __MATERIAL__

#include <glm/glm.hpp>
#include <theia/graphics/shader.h>

namespace theia
{
  struct MaterialState
  {
    MaterialState(ShaderPtr shader)
      : Ke(0), Ka(0.1f), Kd(0.5f), Ks(0.6f, 0.6f, 0.6f, 32),
        shader(shader),
        emissiveParam(shader->GetParameter("Ke")),
        ambientParam(shader->GetParameter("Ka")),
        diffuseParam(shader->GetParameter("Kd")),
        specularParam(shader->GetParameter("Ks"))
    {
    }

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
