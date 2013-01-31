

#include <vector>
#include <new>
#include "../gl_loader.h"
#include <theia/graphics/shaders/shader_program.h>
#include <theia/misc/debug.h>

using namespace theia;
using namespace theia::graphics;

//----------------------------------------------------------------------------------

class ShaderProgram::Impl
{
public:
  Impl() : program(glCreateProgram()) { }

  ~Impl() { glDeleteProgram(program); }

  /// Helper method to compile one stage of a shader program.
  ///
  /// @param[in] src    Nul-terminate source code for the stage.
  /// @param[in] type   The OpenGL enum giving the stage's type (vertex, fragment, geometry).
  ///
  /// @return An Open GL shader object, regardless of whether the source code compiled or not.
  GLuint CompileShaderObject(const char* src, GLenum type);

  /// Helper method to link a set of shader objects into a shader program.
  ///
  /// @param[in] objects    A set of compiled shader objects.
  ///
  /// @return true if the shader program linked successfull, otherwise false.
  bool LinkShaderProgram(std::vector<GLuint>& objects);

//  void EnumerateAttributes();
  void EnumerateUniforms();

  GLuint  program;
};

//----------------------------------------------------------------------------------

ShaderProgram::ShaderProgram()
{
}

//----------------------------------------------------------------------------------

ShaderProgram::~ShaderProgram()
{
  delete impl;
}

//----------------------------------------------------------------------------------

ShaderProgram* ShaderProgram::Create(const char* vertexSrc, const char* fragmentSrc)
{
  return Create(vertexSrc, NULL, fragmentSrc);
}

//----------------------------------------------------------------------------------

ShaderProgram* ShaderProgram::Create(const char* vertexSrc, const char* geometrySrc, const char* fragmentSrc)
{
  ShaderProgram* shader = new (std::nothrow) ShaderProgram();

  std::vector<GLuint> shaderStages;
  shaderStages.push_back(shader->impl->CompileShaderObject(vertexSrc, GL_VERTEX_SHADER));
  if (NULL != geometrySrc) { shaderStages.push_back(shader->impl->CompileShaderObject(geometrySrc, GL_GEOMETRY_SHADER)); }
  shaderStages.push_back(shader->impl->CompileShaderObject(fragmentSrc, GL_FRAGMENT_SHADER));

  if (shader->impl->LinkShaderProgram(shaderStages))
  {

  }
  else
  {
    delete shader;
    shader = NULL;
  }

  for (size_t i = 0; i < shaderStages.size(); ++i)
  {
    glDeleteShader(shaderStages[i]);
  }

  return shader;
}

//----------------------------------------------------------------------------------

GLuint ShaderProgram::Impl::CompileShaderObject(const char* src, GLenum type)
{
  GLuint shaderObject = glCreateShader(type);
  glShaderSource(shaderObject, 1, &src, NULL);
  glCompileShader(shaderObject);

  GLint didCompile;
  glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &didCompile);
  if (!didCompile)
  {
    GLint logLength;
    glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> log(logLength+1);
    glGetShaderInfoLog(shaderObject, logLength, NULL, log.data());
    const char* typeName;
    switch (type)
    {
      case GL_VERTEX_SHADER: typeName = "vertex"; break;
      case GL_FRAGMENT_SHADER: typeName = "fragment"; break;
      case GL_GEOMETRY_SHADER: typeName = "geometry"; break;
    }
    LOG("%s shader compilation failed:\n%s\n", typeName, log.data());
  }

  return shaderObject;
}

//----------------------------------------------------------------------------------

bool ShaderProgram::Impl::LinkShaderProgram(std::vector<GLuint>& objects)
{
  for (size_t i = 0; i < objects.size(); ++i)
  {
    glAttachShader(program, objects[i]);
  }

  glLinkProgram(program);

  GLint didLink;
  glGetProgramiv(program, GL_LINK_STATUS, &didLink);
  if (!didLink)
  {
    GLint logLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> log(logLength+1);
    glGetProgramInfoLog(program, logLength, NULL, log.data());
    LOG("shader program link failed:\n%s\n", log.data());
  }
  
  for (size_t i = 0; i < objects.size(); ++i)
  {
    glDetachShader(program, objects[i]);
  }

  return (0 != didLink);
}

//----------------------------------------------------------------------------------

void ShaderProgram::Impl::EnumerateUniforms()
{
  GLint numUniforms;
  glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
  LOG("there are %d uniforms in the program\n", numUniforms);

  GLint maxNameLength;
  glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);
  std::vector<GLchar> nameBuffer(maxNameLength + 1);

  for (int i = 0; i < numUniforms; ++i)
  {
    GLsizei nameLength;
    GLint size;
    GLenum type;
    glGetActiveUniform(program, i, maxNameLength, &nameLength, &size, &type, nameBuffer.data());

    LOG("uniform #%d at %d is %s\n",
      i, glGetUniformLocation(program, nameBuffer.data()), nameBuffer.data());
  }
}
