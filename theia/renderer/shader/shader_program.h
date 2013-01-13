/// A shader program is a collection of linked shader objects.

#if ! defined(__THEIA_SHADER_PROGRAM__)
#define __THEIA_SHADER_PROGRAM__

#include <map>
#include <vector>
#include <renderer/shader/shader_object.h>
#include <renderer/shader/vertex_attribute.h>

namespace theia
{
  namespace renderer
  {
    struct ShaderProgram
    {
      virtual ~ShaderProgram();

      const unsigned int Program;

      void AddAttribute(const VertexAttribute* attr);

      static ShaderProgram* Create(std::vector<ShaderObject*>& objects);

    private:
      ShaderProgram(unsigned int program);

      std::map<const char* const, VertexAttribute*>* attributes;
    };
  }
}

#endif // __THEIA_SHADER_PROGRAM__
