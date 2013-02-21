#include <new>
#include <memory>
#include <vector>
#include <string.h>
#include <gl_loader.h>
#include <shader.h>
#include <debug.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

//--------------------------------------------------------------------------------

struct Shader::Parameter
{
  GLuint location;  // as given by the "layout (location = n)" attribute declaration
  GLenum type;      // the type of the data stored in the attribute (gleaned from the shader program itself)
  float data[16];   // a CPU-side cache of the data for quick recall and to prevent unnecessary GL calls
  bool dirty;       // whether the cache is out of step with the GPU side, forcing an upload on Shader::Apply if it is
  GLchar name[32];  // the name of the parameter (gleaned from the shader program itself)
};

struct Shader::Impl
{
  Impl()
    : program(0), numParams(0), params(NULL)
  { }

  ~Impl()
  {
    delete params;
    glDeleteProgram(program);
  }

  // the GL handle to the shader program
  GLuint program;

  // array of the parameters
  size_t numParams;
  Shader::Parameter* params;
};

//--------------------------------------------------------------------------------

// These definitions are pre-pended to all shader source code to provide various
// macros and functions.
static const char* const shaderMacros =
  "#version 330\n"
  // Pre-defined vertex attribute locations...
  "#define VERTEX_POSITION	layout(location = 0)\n"
  "#define VERTEX_NORMAL    layout(location = 1)\n"
  "#define VERTEX_TEXCOORD  layout(location = 2)\n"
  "#define VERTEX_COLOUR		layout(location = 3)\n"
  // Math helpers...
  "#define PI 3.141592f\n"
  "#define TWO_PI (2.0f * PI)\n"
  "#define HALF_PI (0.5f * PI)\n"
  ;

//--------------------------------------------------------------------------------

static GLuint CompileShader(GLenum type, const char* const src)
{
  // Create the compilation unit by combining the built-in symbols with the given source
  // code. The #version line in the original source is skipped to ensure that our's is
  // the first non-comment line in the compilation.
  const char* compilationUnits[2];
  compilationUnits[0] = shaderMacros;

  // Skip the #version line in the user's source code (not an error if it isn't there)...
  const char* pos = strstr(src, "#version");
  if (NULL != pos)
  {
    // Go to the end of the line...
    pos = strchr(pos, '\n');
    // Set the compilation unit to start from the end of the #version line...
    compilationUnits[1] = pos;
  }
  else
  {
    // They didn't provide a #version line - just suck in the whole text...
    compilationUnits[1] = src;
  }

  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 2, compilationUnits, NULL);
  glCompileShader(shader);
  GLint didCompile;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &didCompile);
  if (!didCompile)
  {
    GLint logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> log(logLength+1);
    glGetShaderInfoLog(shader, logLength, NULL, log.data());
    const char* typeName;
    switch (type)
    {
      case GL_VERTEX_SHADER: typeName = "vertex"; break;
      case GL_FRAGMENT_SHADER: typeName = "fragment"; break;
      case GL_GEOMETRY_SHADER: typeName = "geometry"; break;
    }
    LOG("%s shader compilation failed:\n%s\n", typeName, log.data());
  }
  return shader;
}

//--------------------------------------------------------------------------------

static bool LinkShader(GLuint shader, GLuint parts[], size_t numParts)
{
  for (size_t i = 0; i < numParts; ++i) { glAttachShader(shader, parts[i]); }

  glLinkProgram(shader);

  GLint didLink;
  glGetProgramiv(shader, GL_LINK_STATUS, &didLink);
  if (!didLink)
  {
    GLint logLength;
    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> log(logLength+1);
    glGetProgramInfoLog(shader, logLength, NULL, log.data());
    LOG("shader shader link failed:\n%s\n", log.data());
  }
  
  for (size_t i = 0; i < numParts; ++i) { glDetachShader(shader, parts[i]); }

  return (0 != didLink);
}

//--------------------------------------------------------------------------------

static void EnumerateUniforms(GLuint program, Shader::Parameter* params, size_t numParams)
{
  // Get information about the active shader uniform values...
  std::vector<GLuint> indices(numParams);
  std::vector<GLint>  nameLengths(numParams);
  std::vector<GLint>  blockIndices(numParams);
  std::vector<GLint>  types(numParams);
  for (size_t i = 0; i < numParams; ++i) { indices[i] = i; }
  glGetActiveUniformsiv(program, numParams, indices.data(), GL_UNIFORM_BLOCK_INDEX, blockIndices.data());
  glGetActiveUniformsiv(program, numParams, indices.data(), GL_UNIFORM_NAME_LENGTH, nameLengths.data());
  glGetActiveUniformsiv(program, numParams, indices.data(), GL_UNIFORM_TYPE, types.data());

  for (size_t i = 0; i < numParams; ++i)
  {
    // Only handling non block-based variables for now...
    if (-1 == blockIndices[i])
    {
      glGetActiveUniformName(program, i, sizeof(params[0].name) - 1, NULL, params[i].name);
      params[i].location = glGetUniformLocation(program, params[i].name);
      params[i].type = types[i];
    }
  }
}

//--------------------------------------------------------------------------------

ShaderPtr Shader::Factory::New()
{
  ShaderPtr shader( new Shader() );
  return shader;
}

//--------------------------------------------------------------------------------

Shader::Shader()
  : impl(new Impl())
{
}

//--------------------------------------------------------------------------------

Shader::~Shader()
{
  delete impl;
}

//--------------------------------------------------------------------------------

bool Shader::Compile(const char* const vertexShader, const char* fragmentShader)
{
  bool compiled = false;
  impl->program = glCreateProgram();

  GLuint parts[2] =
  {
    CompileShader(GL_VERTEX_SHADER, vertexShader),
    CompileShader(GL_FRAGMENT_SHADER, fragmentShader)
  };

  if (LinkShader(impl->program, parts, 2))
  {
    compiled = true;

    glGetProgramiv(impl->program, GL_ACTIVE_UNIFORMS, (GLint*)&impl->numParams);
    if (impl->numParams > 0)
    {
      impl->params = new Shader::Parameter[impl->numParams];    
      EnumerateUniforms(impl->program, impl->params, impl->numParams);
    }
  }

  // Don't need the temporary shader parts...
  glDeleteShader(parts[0]);
  glDeleteShader(parts[1]);

  return compiled;
}

//--------------------------------------------------------------------------------

void Shader::Apply()
{
  glUseProgram(impl->program);

  for (size_t i = 0; i < impl->numParams; ++i)
  {
    if (impl->params[i].dirty)
    {
      switch (impl->params[i].type)
      {
      case GL_FLOAT:      glUniform1fv(impl->params[i].location, 1, impl->params[i].data); break;
      case GL_FLOAT_VEC2: glUniform2fv(impl->params[i].location, 1, impl->params[i].data); break;
      case GL_FLOAT_VEC3: glUniform3fv(impl->params[i].location, 1, impl->params[i].data); break;
      case GL_FLOAT_VEC4: glUniform4fv(impl->params[i].location, 1, impl->params[i].data); break;
      case GL_FLOAT_MAT4: glUniformMatrix4fv(impl->params[i].location, 1, GL_FALSE, impl->params[i].data); break;
      default: ASSERT(false); break;
      }
      impl->params[i].dirty = false;
    }
  }
}

//--------------------------------------------------------------------------------

Shader::Parameter* const Shader::GetParameter(const char* const name)
{
  for (size_t i = 0; i < impl->numParams; ++i)
  {
    if (0 == strcmp(impl->params[i].name, name))
    {
      return &impl->params[i];
    }
  }

  ASSERTM(false, "unknown parameter name '%s'\n", name);

  return NULL;
}

//--------------------------------------------------------------------------------

static void CacheParameter(Shader::Parameter* param, const float* value, size_t size)
{
  if (0 != memcmp(param->data, value, size))
  {
    memcpy(param->data, value, size);
    param->dirty = true;
  }
}

//--------------------------------------------------------------------------------

void Shader::SetParameter(Shader::Parameter* param, float value)
{
  CacheParameter(param, &value, sizeof(value));
}
void Shader::SetParameter(Shader::Parameter* param, const glm::vec2& value)
{
  CacheParameter(param, glm::value_ptr(value), sizeof(value));
}
void Shader::SetParameter(Shader::Parameter* param, const glm::vec3& value)
{
  CacheParameter(param, glm::value_ptr(value), sizeof(value));
}
void Shader::SetParameter(Shader::Parameter* param, const glm::vec4& value)
{
  CacheParameter(param, glm::value_ptr(value), sizeof(value));
}

//--------------------------------------------------------------------------------
void Shader::SetParameter(Parameter* const param, const glm::mat4& value)
{
  CacheParameter(param, glm::value_ptr(value), sizeof(value));
}
