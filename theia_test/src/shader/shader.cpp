#include <memory>
#include <vector>
#include <string.h>
#include <gl_loader.h>
#include <shader.h>
#include <debug.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

//--------------------------------------------------------------------------------
struct ShaderParameter
{
  GLuint location;  // as given by the "layout (location = n)" attribute declaration
  GLenum type;      // the type of the data stored in the attribute (gleaned from the shader program itself)
  float data[16];   // a CPU-side cache of the data for quick recall and to prevent unnecessary GL calls
  bool dirty;       // whether the cache is out of step with the GPU side, forcing an upload on Shader::Apply if it is
  GLchar name[32];  // the name of the parameter (gleaned from the shader program itself)
};

//--------------------------------------------------------------------------------

const unsigned int MaxShaders = 1024;
const unsigned int MaxShaderParameters = 4096;

static unsigned int nextShader = 0;
static unsigned int nextShaderParam = 0;

static Shader shaderPool[MaxShaders] = { 0 };
static ShaderParameter  shaderParamPool[MaxShaderParameters] = { 0 };

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

static void EnumerateUniforms(Shader* shader)
{
  glGetProgramiv(shader->program, GL_ACTIVE_UNIFORMS, (GLint*)&shader->numParams);

  ASSERT(shader->numParams < Shader::MaxParameters, "shader struct needs at least %d params\n", shader->numParams);

  if (shader->numParams > 0)
  {
    // Get information about the active shader uniform values...
    std::vector<GLuint> indices(shader->numParams);
    std::vector<GLint>  nameLengths(shader->numParams);
    std::vector<GLint>  blockIndices(shader->numParams);
    std::vector<GLint>  types(shader->numParams);
    for (size_t i = 0; i < shader->numParams; ++i) { indices[i] = i; }
    glGetActiveUniformsiv(shader->program, shader->numParams, indices.data(), GL_UNIFORM_BLOCK_INDEX, blockIndices.data());
    glGetActiveUniformsiv(shader->program, shader->numParams, indices.data(), GL_UNIFORM_NAME_LENGTH, nameLengths.data());
    glGetActiveUniformsiv(shader->program, shader->numParams, indices.data(), GL_UNIFORM_TYPE, types.data());

    for (size_t i = 0; i < shader->numParams; ++i)
    {
      // Only handling non block-based variables for now...
      if (-1 == blockIndices[i])
      {
        shader->params[i] = nextShaderParam;
        ShaderParameter* p = &shaderParamPool[nextShaderParam];
        ++nextShaderParam;

        glGetActiveUniformName(shader->program, i, sizeof(p->name) - 1, NULL, p->name);
        p->location = glGetUniformLocation(shader->program, p->name);
        p->type = types[i];
      }
    }
  }
}

//--------------------------------------------------------------------------------

static Shader* AllocateNewShader()
{
  Shader* shader = NULL;

  ASSERT(nextShader < MaxShaders, "increase MaxShaders\n");
  shader = &shaderPool[nextShader];
  ++nextShader;

  return shader;
}

//--------------------------------------------------------------------------------

Shader* Shaders::Create(const char* const vertexShader, const char* fragmentShader)
{
  Shader* shader = AllocateNewShader();

  shader->program = glCreateProgram();

  GLuint parts[2] =
  {
    CompileShader(GL_VERTEX_SHADER, vertexShader),
    CompileShader(GL_FRAGMENT_SHADER, fragmentShader)
  };

  if (LinkShader(shader->program, parts, 2))
  {
    EnumerateUniforms(shader);
  }

  // Don't need the temporary shader parts...
  glDeleteShader(parts[0]);
  glDeleteShader(parts[1]);

  return shader;
}

//--------------------------------------------------------------------------------

void Shaders::DestroyAll()
{
  for (unsigned short i = 0; i < nextShader; ++i)
  {
    glDeleteProgram(shaderPool[i].program);
  }

  memset(shaderPool, 0, sizeof(shaderPool));
  memset(shaderParamPool, 0, sizeof(shaderParamPool));

  nextShader = 0;
  nextShaderParam = 0;
}

//--------------------------------------------------------------------------------

void Shaders::Apply(Shader* const shader)
{
  glUseProgram(shader->program);
  for (size_t i = 0; i < shader->numParams; ++i)
  {
    ShaderParameter* p = &shaderParamPool[shader->params[i]];
    if (p->dirty)
    {
      switch (p->type)
      {
      case GL_FLOAT:      glUniform1fv(p->location, 1, p->data); break;
      case GL_FLOAT_VEC2: glUniform2fv(p->location, 1, p->data); break;
      case GL_FLOAT_VEC3: glUniform3fv(p->location, 1, p->data); break;
      case GL_FLOAT_VEC4: glUniform4fv(p->location, 1, p->data); break;
      default: ASSERT(false); break;
      }
      p->dirty = false;
    }
  }
}

//--------------------------------------------------------------------------------

Shaders::Parameter Shaders::GetParameter(const Shader* const shader, const char* const name)
{
  for (unsigned int i = 0; i < shader->numParams; ++i)
  {
    unsigned short paramPoolIndex = shader->params[i];
    if (0 == strcmp(shaderParamPool[paramPoolIndex].name, name))
    {
      return paramPoolIndex;
    }
  }

  ASSERTM(false, "unknown parameter name '%s'\n", name);

  return 0;
}

//--------------------------------------------------------------------------------

void Shaders::SetParameter(Shader* const shader, Parameter param, const glm::vec2& value)
{
  ShaderParameter* p = &shaderParamPool[param];

  if (0 != memcmp(p->data, glm::value_ptr(value), sizeof(value)))
  {
    memcpy(p->data, glm::value_ptr(value), sizeof(value));
    p->dirty = true;
  }
}
