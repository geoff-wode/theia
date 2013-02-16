

#if ! defined( __SHADER__ )
#define __SHADER__

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <glm/glm.hpp>
#include <string>
#include <gl_loader.h>

//--------------------------------------------------------------------------------

class Shader;
typedef boost::shared_ptr<Shader> ShaderPtr;

class Shader
{
public:  
  struct Factory
  {
    static ShaderPtr New();
  };
  
  ~Shader();

  bool Compile(const char* const vertexShader, const char* fragmentShader);

  void Apply();

  struct Parameter;
  Parameter* const GetParameter(const char* const name);

  void SetParameter(Parameter* const param, float value);
  void SetParameter(Parameter* const param, const glm::vec2& value);
  void SetParameter(Parameter* const param, const glm::mat4& value);

private:
  Shader();

  struct Impl;
  Impl* impl;
};

#endif // __SHADER__
