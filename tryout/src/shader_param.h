#if ! defined(__SHADER_PARAM__)
#define __SHADER_PARAM__

#include <map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gl_loader.h>

class ShaderParam
{
public:
  ShaderParam(GLuint shader, const char* name)
    : Location(glGetUniformLocation(shader, name)), Name(name)
  {
  }

  const GLuint Location;
  const std::string Name;
};

#endif // __SHADER_PARAM__
