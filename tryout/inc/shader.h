#if ! defined(__SHADER__)
#define __SHADER__

#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include <gl_loader.h>
#include <shader_param.h>

//--------------------------------------------------

class Shader
{
public:
  typedef boost::shared_ptr<Shader> ShaderPtr;

  // Build a shader from vertex and fragment stages.
  // @return A new object or NULL on error.
  static ShaderPtr Create(const char* vs, const char* fs);

  ~Shader();

  // Make the shader active (or not).
  void Activate() const;
  void Deactivate() const;

  typedef std::map<std::string, ShaderParamPtr> ShaderParamList;
  ShaderParamList&  GetParameters() const;

private:
  Shader();

  class Impl;
  boost::shared_ptr<Impl> impl;
};

typedef boost::shared_ptr<Shader> ShaderPtr;

#endif // __SHADER__
