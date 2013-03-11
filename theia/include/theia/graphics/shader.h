/// Declare a GPU shader.

#if ! defined(__THEIA_GFX_SHADER__)
#define __THEIA_GFX_SHADER__

#include <stdint.h>
#include <glm/glm.hpp>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <theia/graphics/gl/gl_loader.h>

namespace theia
{
  struct Shader;
  typedef boost::shared_ptr<Shader> ShaderPtr;

  struct Shader
  {
    struct Parameter
    {
      bool dirty;       // whether the cache is out of step with the GPU side, forcing an upload on Shader::Apply if it is
      GLuint location;  // as given by the "layout (location = n)" attribute declaration
      GLenum type;      // the type of the data stored in the attribute (gleaned from the shader program itself)
      uint8_t data[16 * sizeof(double)]; // a CPU-side cache of the data for quick recall and to prevent unnecessary GL calls
      GLchar name[32];  // the name of the parameter (gleaned from the shader program itself)
    };

    Shader();
    ~Shader();

    /// Compile a shader program containing a vertex and fragment stage.
    ///
    /// @param[in] vertexSrc    Pointer to the nul-terminated source code of the vertex shader stage source code.
    /// @param[in] fragmentSrc  Pointer to the nul-terminated source code of the fragment shader stage source code.
    ///
    /// @return true if compilation succeeded, otherwise false.
    bool Compile(const char* commonSrc, const char* vertexSrc, const char* fragmentSrc);
    bool Compile(uint32_t commonResource, uint32_t vertexShaderResource, uint32_t fragmentShaderResource);

    /// Make this shader active and copy all modified parameter values to the GPU.
    void Activate();

    Parameter* const GetParameter(const char* const name);

    void SetParameter(Parameter* const param, float value);
    void SetParameter(Parameter* const param, const glm::vec2& value);
    void SetParameter(Parameter* const param, const glm::vec3& value);
    void SetParameter(Parameter* const param, const glm::vec4& value);
    void SetParameter(Parameter* const param, const glm::mat3& value);
    void SetParameter(Parameter* const param, const glm::mat4& value);

    void SetParameter(Parameter* const param, double value);
    void SetParameter(Parameter* const param, const glm::dvec2& value);
    void SetParameter(Parameter* const param, const glm::dvec3& value);
    void SetParameter(Parameter* const param, const glm::dvec4& value);
    void SetParameter(Parameter* const param, const glm::dmat3& value);
    void SetParameter(Parameter* const param, const glm::dmat4& value);


    GLuint program;
    std::vector<Parameter> params;
  };
}

#endif // __THEIA_GFX_SHADER__
