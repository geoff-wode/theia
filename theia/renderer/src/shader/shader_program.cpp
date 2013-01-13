

#include <renderer/gl/gl_loader.h>
#include <renderer/shader/shader_program.h>

using namespace theia;
using namespace theia::renderer;

//-----------------------------------------------------------------------------

ShaderProgram::ShaderProgram(unsigned int program)
  : Program(program)
{
  attributes = new std::map<const char* const, VertexAttribute*>();
}

ShaderProgram::~ShaderProgram()
{
  if (0 != Program)
  {
    delete attributes;
    glDeleteProgram(Program);
  }
}

//-----------------------------------------------------------------------------

ShaderProgram* ShaderProgram::Create(std::vector<ShaderObject*>& objects)
{
  ShaderProgram* program = NULL;
  unsigned int glProgram = glCreateProgram();

  for (unsigned int i = 0; i < objects.size(); ++i)
  {
    glAttachShader(glProgram, objects[i]->Shader);
  }

  glLinkProgram(glProgram);

  int ok;
  glGetProgramiv(glProgram, GL_LINK_STATUS, &ok);

  if (ok)
  {
    program = new ShaderProgram(glProgram);
  }
  else
  {
    glDeleteProgram(glProgram);
    program = 0;
    int logLength;
    glGetProgramiv(glProgram, GL_INFO_LOG_LENGTH, &logLength);
    char* log = (char*)malloc(logLength);
    glGetShaderInfoLog(glProgram, logLength, &logLength, log);
    fprintf(stderr, "*shader link error:\n%s", log);
    free(log);
  }

  return program;
}

//-----------------------------------------------------------------------------

void ShaderProgram::AddAttribute(const VertexAttribute* attr)
{
  (*attributes)[attr->Name()] = (VertexAttribute*)attr;

  (*attributes)[attr->Name()]->location = glGetAttribLocation(Program, attr->Name());
}
