#include <algorithm>
#include <vector>
#include <shader.h>

//----------------------------------------------

class Shader::Impl
{
public:
  GLuint obj;
  Shader::ShaderParamList paramList;

  Impl() : obj(glCreateProgram()) { }
  ~Impl() { glDeleteProgram(obj); }
};

//----------------------------------------------

static GLuint CompileShader(const char* src, GLenum type);
static bool LinkShader(GLuint shader, std::vector<GLuint>& objects);
static void GetUniforms(GLuint shader, Shader::ShaderParamList& params);

//----------------------------------------------
Shader::ShaderPtr Shader::Create(const char* vs, const char* fs)
{
  ShaderPtr shader(new Shader());

  std::vector<GLuint> stages;
  stages.push_back(CompileShader(vs, GL_VERTEX_SHADER));
  stages.push_back(CompileShader(fs, GL_FRAGMENT_SHADER));

  if (LinkShader(shader->impl->obj, stages))
  {
    shader->Activate();
    GetUniforms(shader->impl->obj, shader->impl->paramList);
    shader->Deactivate();
  }
  else
  {
    shader.reset();
    shader = NULL;
  }

  std::for_each(stages.begin(), stages.end(), glDeleteShader);

  return shader;
}

//----------------------------------------------
Shader::Shader()
  : impl(new Shader::Impl())
{
}

//----------------------------------------------
Shader::~Shader()
{
}

//----------------------------------------------
void Shader::Activate() const
{
  glUseProgram(impl->obj);
}

//----------------------------------------------
void Shader::Deactivate() const
{
  glUseProgram(0);
}

//----------------------------------------------
Shader::ShaderParamList& Shader::GetParameters() const
{
  return impl->paramList;
}

//----------------------------------------------------------
static GLuint CompileShader(const char* src, GLenum type)
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
    fprintf(stderr,"%s shader compilation failed:\n%s\n", typeName, log.data());
  }

  return shaderObject;
}

//----------------------------------------------------------
static bool LinkShader(GLuint program, std::vector<GLuint>& objects)
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
    fprintf(stderr, "shader program link failed:\n%s\n", log.data());
  }
  
  for (size_t i = 0; i < objects.size(); ++i)
  {
    glDetachShader(program, objects[i]);
  }

  return (0 != didLink);
}

//----------------------------------------------------------
static void GetUniforms(GLuint shader, Shader::ShaderParamList& params)
{
  GLint numUniforms = 0;
  glGetProgramiv(shader, GL_ACTIVE_UNIFORMS, &numUniforms);

  std::vector<GLuint> indices(numUniforms);
  std::vector<GLint>  nameLengths(numUniforms);
  std::vector<GLint>  blockIndices(numUniforms);

  for (int i = 0; i < numUniforms; ++i)
  {
    indices[i] = i;
  }
  glGetActiveUniformsiv(shader, numUniforms, indices.data(), GL_UNIFORM_BLOCK_INDEX, blockIndices.data());
  glGetActiveUniformsiv(shader, numUniforms, indices.data(), GL_UNIFORM_NAME_LENGTH, nameLengths.data());

  for (int i = 0; i < numUniforms; ++i)
  {
    if (blockIndices[i] == -1)  // Uniform is not a block...
    {
      std::vector<char> buffer(nameLengths[i]);
      glGetActiveUniformName(shader, i, buffer.size(), NULL, buffer.data());

      ShaderParamPtr p(new ShaderParam(shader, buffer.data()));

      std::string name(buffer.data());
      params[name] = p;
    }
  }
}
