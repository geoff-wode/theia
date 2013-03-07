
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <theia/graphics/gl/gl_loader.h>
#include <theia/graphics/shader.h>
#include <theia/misc/debug.h>
#include <theia/resource_loader.h>

using namespace theia;

//--------------------------------------------------------------------------------

static GLuint CompileShader(GLenum type, const char* commonSrc, const char* const src);
static bool LinkShader(GLuint shader, GLuint parts[], size_t numParts);
static void EnumerateUniforms(GLuint program, std::vector<Shader::Parameter>& params);

//--------------------------------------------------------------------------------

Shader::Shader()
  : program(glCreateProgram())
{
}

Shader::~Shader()
{
  glDeleteProgram(program);
}

bool Shader::Compile(const char* commonSrc, const char* vertexSrc, const char* fragmentSrc)
{
  bool compiled = false;

  GLuint parts[2] =
  {
    CompileShader(GL_VERTEX_SHADER, commonSrc, vertexSrc),
    CompileShader(GL_FRAGMENT_SHADER, commonSrc, fragmentSrc)
  };

  if (LinkShader(program, parts, 2))
  {
    compiled = true;
    GLint numParams;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numParams);
    if (numParams > 0)
    {
      params.resize(numParams);
      EnumerateUniforms(program, params);
    }
  }

  // Don't need the temporary shader parts...
  glDeleteShader(parts[0]);
  glDeleteShader(parts[1]);

  return compiled;
}

bool Shader::Compile(uint32_t commonResource, uint32_t vertexShaderResource, uint32_t fragmentShaderResource)
{
  Resource shaderCommon;
  Resource vertexShader;
  Resource fragmentShader;

  ResourceLoader::Load(commonResource, 256, shaderCommon);
  ResourceLoader::Load(vertexShaderResource, 256, vertexShader);
  ResourceLoader::Load(fragmentShaderResource, 256, fragmentShader);

  if (shaderCommon.data && vertexShader.data && fragmentShader.data)
  {
    return Compile((const char*)shaderCommon.data, (const char*)vertexShader.data, (const char*)fragmentShader.data);
  }
  return false;
}

void Shader::Activate()
{
  glUseProgram(program);

  for (size_t i = 0; i < params.size(); ++i)
  {
    if (params[i].dirty)
    {
      switch (params[i].type)
      {
      case GL_FLOAT:      glUniform1fv(params[i].location, 1, params[i].data); break;
      case GL_FLOAT_VEC2: glUniform2fv(params[i].location, 1, params[i].data); break;
      case GL_FLOAT_VEC3: glUniform3fv(params[i].location, 1, params[i].data); break;
      case GL_FLOAT_VEC4: glUniform4fv(params[i].location, 1, params[i].data); break;
      case GL_FLOAT_MAT3: glUniformMatrix3fv(params[i].location, 1, GL_FALSE, params[i].data); break;
      case GL_FLOAT_MAT4: glUniformMatrix4fv(params[i].location, 1, GL_FALSE, params[i].data); break;
      default: ASSERT(false); break;
      }
      params[i].dirty = false;
    }
  }
}

Shader::Parameter* const Shader::GetParameter(const char* const name)
{
  for (size_t i = 0; i < params.size(); ++i)
  {
    if (0 == strcmp(params[i].name, name))
    {
      return &params[i];
    }
  }

  LOG("unknown parameter name '%s'\n", name);

  return NULL;
}

//--------------------------------------------------------------------------------

static void CacheParameter(Shader::Parameter* const param, const float* value, size_t size)
{
  if (param && (0 != memcmp(param->data, value, size)))
  {
    memcpy(param->data, value, size);
    param->dirty = true;
  }
}

//--------------------------------------------------------------------------------

void Shader::SetParameter(Parameter* const param, float value)
{
  CacheParameter(param, &value, sizeof(value));
}
void Shader::SetParameter(Parameter* const param, const glm::vec2& value)
{
  CacheParameter(param, glm::value_ptr(value), sizeof(value));
}
void Shader::SetParameter(Parameter* const param, const glm::vec3& value)
{
  CacheParameter(param, glm::value_ptr(value), sizeof(value));
}
void Shader::SetParameter(Parameter* const param, const glm::vec4& value)
{
  CacheParameter(param, glm::value_ptr(value), sizeof(value));
}
void Shader::SetParameter(Parameter* const param, const glm::mat3& value)
{
  CacheParameter(param, glm::value_ptr(value), sizeof(value));
}
void Shader::SetParameter(Parameter* const param, const glm::mat4& value)
{
  CacheParameter(param, glm::value_ptr(value), sizeof(value));
}

//--------------------------------------------------------------------------------

static GLuint CompileShader(GLenum type, const char* common, const char* const src)
{
  const char* compilationUnits[2];
  compilationUnits[0] = common;

  // Skip the #version line in the user's source code (not an error if it isn't there)...
  const char* pos = strstr(src, "#version");
  if (NULL != pos)
  {
    // Set the compilation unit to start from the end of the #version line...
    compilationUnits[1] = strchr(pos, '\n');
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
    LOG("%s\n", log.data());
    exit(EXIT_FAILURE);
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
    LOG("%s\n", log.data());
    exit(EXIT_FAILURE);
  }
  
  for (size_t i = 0; i < numParts; ++i) { glDetachShader(shader, parts[i]); }

  return (0 != didLink);
}

//--------------------------------------------------------------------------------

static void EnumerateUniforms(GLuint program, std::vector<Shader::Parameter>& params)
{
  const size_t numParams = params.capacity();

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
