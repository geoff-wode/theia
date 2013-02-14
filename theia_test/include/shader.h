

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

private:
  Shader();

  struct Impl;
  Impl* impl;
};

//namespace Shaders
//{
//  typedef unsigned short Parameter;
//
//  ShaderPtr Create(const char* const vertexShader, const char* fragmentShader);
//  void    DestroyAll();
//
//  void    Apply(ShaderPtr const shader);
//
//  Parameter GetParameter(const ShaderPtr const shader, const char* const name);
//
//  void SetParameter(ShaderPtr const shader, Parameter param, float value);
//  void SetParameter(ShaderPtr const shader, Parameter param, const glm::vec2& value);
//}

#endif // __SHADER__
