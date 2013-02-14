

#if ! defined( __SHADER__ )
#define __SHADER__

#include <glm/glm.hpp>
#include <string>
#include <gl_loader.h>

struct Shader
{
  GLuint program;             // the GL handle to the shader program
  size_t numParams;           // number of parameters the shader program has

  static const unsigned int MaxParameters = 128;
  unsigned short params[MaxParameters]; // a list of the parameters
};

namespace Shaders
{
  typedef unsigned short Parameter;

  Shader* Create(const char* const vertexShader, const char* fragmentShader);
  void    DestroyAll();

  void    Apply(Shader* const shader);

  Parameter GetParameter(const Shader* const shader, const char* const name);

  void SetParameter(Shader* const shader, Parameter param, const glm::vec2& value);
}

#endif // __SHADER__
