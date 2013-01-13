
#include <stdio.h>
#include <renderer/gl/gl_loader.h>
#include <renderer/shader/shader_object.h>

using namespace theia;
using namespace theia::renderer;

//-----------------------------------------------------------------------------

static bool CompileShader(const char* src, GLenum type, unsigned int* shader);

//-----------------------------------------------------------------------------

ShaderObject::ShaderObject(ShaderType::e type, unsigned int shader)
  : Type(type), Shader(shader)
{
}

//-----------------------------------------------------------------------------

ShaderObject::~ShaderObject()
{
  if (0 != Shader)
  {
    glDeleteShader(Shader);
  }
}

//-----------------------------------------------------------------------------

ShaderObject* ShaderObject::Compile(ShaderType::e type, const char* src)
{
  ShaderObject* shaderObj = NULL;
  unsigned int shader = 0;
  GLenum glType = 0;

  switch (type)
  {
  case ShaderType::Vertex:
    glType = GL_VERTEX_SHADER;
    break;
  case ShaderType::Geometry:
    glType = GL_GEOMETRY_SHADER;
    break;
  case ShaderType::Fragment:
    glType = GL_FRAGMENT_SHADER;
    break;
  default:
    break;
  }

  if (CompileShader(src, glType, &shader))
  {
    shaderObj = new ShaderObject(type, shader);
  }

  return shaderObj;
}

//-----------------------------------------------------------------------------

static bool CompileShader(const char* src, GLenum type, unsigned int* shader)
{
  *shader = glCreateShader(type);

  const int strLen = strlen(src);
  glShaderSource(*shader, 1, &src, &strLen);
  glCompileShader(*shader);
  int ok;
  glGetShaderiv(*shader, GL_COMPILE_STATUS, &ok);
  if (!ok)
  {
    int logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    char* log = (char*)malloc(logLength);
    glGetShaderInfoLog(*shader, logLength, &logLength, log);
    fprintf(stderr, "*shader compiler error:\n%s", log);
    free(log);
  }

  return (0 != *shader);
}
