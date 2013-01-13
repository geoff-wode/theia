#if ! defined(__THEIA_SHADER__)
#define __THEIA_SHADER__

#include <stdexcept>
#include <string>

namespace theia
{
  namespace renderer
  {
    namespace ShaderType
    {
      enum e
      {
        Vertex,
        Geometry,
        Fragment
      };
    }

    struct ShaderObject
    {
      virtual ~ShaderObject();

      const unsigned int  Shader;
      const ShaderType::e Type;

      static ShaderObject* Compile(ShaderType::e type, const char* src);

    private:
      ShaderObject(ShaderType::e type, unsigned int shader);
      ShaderObject(const ShaderObject&);
      ShaderObject& operator=(const ShaderObject&);
    };
  }
}

#endif // __THEIA_SHADER__
