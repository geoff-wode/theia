/**
Implementation of the mesh object.
*/

#include <map>
#include <SDL.h>
#include <algorithm>
#include <stdlib.h>
#include <stddef.h>
#include <memory>
#include <vector>
#include <gl_loader.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../resources/resource.h"
#include "shader_param.h"

//----------------------------------------------------------

class Device
{
public:
  Device()
    : Width(800), Height(600)
  {
    SDL_Init(SDL_INIT_EVERYTHING);
  }
  ~Device()
  {
    SDL_Quit();
  }

  void Init()
  {
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    if (NULL == SDL_SetVideoMode(Width, Height, 32, SDL_OPENGL | SDL_HWSURFACE))
    {
      fprintf(stderr, "device init failed - %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
    }

    ogl_LoadFunctions();
    fprintf(stderr, "GL v%d.%d loaded\n", ogl_GetMajorVersion(), ogl_GetMinorVersion());

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
  }

  size_t  Width;
  size_t  Height;
};

//----------------------------------------------------------

namespace VertexElementUsage
{
  enum Enum
  {
    Position,
    Colour
  };
}

struct VertexDeclaration
{
  const VertexElementUsage::Enum Usage;
  const GLuint  NumComponents;
  const GLenum  BaseType;
  const GLsizei Offset;

  VertexDeclaration(VertexElementUsage::Enum usage, GLuint numComponents, GLenum baseType, GLsizei offset)
    : Usage(usage), NumComponents(numComponents), BaseType(baseType), Offset(offset)
  {
  }
};

//----------------------------------------------------------

struct VertexPosition
{
  VertexPosition(const glm::vec3& p, const glm::vec4& c) : pos(p), colour(c) { }

  glm::vec3 pos;
  glm::vec4 colour;

  static size_t Stride;
  static VertexDeclaration VertexDecl[];
  static size_t NumElements;
};
size_t VertexPosition::Stride = sizeof(VertexPosition);
VertexDeclaration VertexPosition::VertexDecl[] =
{
  VertexDeclaration(VertexElementUsage::Position, 3, GL_FLOAT, offsetof(VertexPosition, pos)),
  VertexDeclaration(VertexElementUsage::Colour, 4, GL_FLOAT, offsetof(VertexPosition, colour)),
};
size_t VertexPosition::NumElements = sizeof(VertexPosition::VertexDecl)/sizeof(VertexPosition::VertexDecl[0]);

//----------------------------------------------------------

static const VertexPosition Vertices[] =
{
  VertexPosition(glm::vec3(-1, 1, 1), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),    // 0
  VertexPosition(glm::vec3(-1,-1, 1), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),    // 1
  VertexPosition(glm::vec3( 1,-1, 1), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)),    // 2
  VertexPosition(glm::vec3( 1, 1, 1), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)),    // 3
  VertexPosition(glm::vec3( 1, 1,-1), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)),    // 4
  VertexPosition(glm::vec3( 1,-1,-1), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)),    // 5
  VertexPosition(glm::vec3(-1,-1,-1), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),    // 6
  VertexPosition(glm::vec3(-1, 1,-1), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f))     // 7
};
static const size_t NumVertices = sizeof(Vertices)/sizeof(Vertices[0]);

static const unsigned short Indices[] =
{
  0, 1, 2,    2, 3, 0,    // Front
  3, 2, 5,    5, 4, 3,    // Right
  4, 5, 6,    6, 7, 4,    // Back
  7, 6, 1,    1, 0, 7,    // Left
  0, 3, 4,    4, 7, 0,    // Top
  1, 6, 5,    5, 2, 1     // Bottom
};
static const size_t NumIndices = sizeof(Indices)/sizeof(Indices[0]);

//----------------------------------------------------------

static GLuint CreateVertexBuffer(size_t vertexCount, size_t vertexStride, const void* const data)
{
  GLuint vb;
  glGenBuffers(1, &vb);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexStride, data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return vb;
}

//----------------------------------------------------------

static GLuint CreateIndexBuffer(size_t size, const void* const data)
{
  GLuint ib;
  glGenBuffers(1, &ib);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  return ib;
}

//----------------------------------------------------------

static void KeyboardHandler(SDL_Event& event)
{
  if (SDL_KEYDOWN == event.type)
  {
    if (SDLK_ESCAPE == event.key.keysym.sym)
    {
      event.type = SDL_QUIT;
      SDL_PushEvent(&event);
    }
  }
  else
  {
  }
}

//----------------------------------------------------------
static void EventHandler(bool& stop)
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      stop = true;
      break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      KeyboardHandler(event);
      break;
    default:
      break;
    }
  }
}

//----------------------------------------------------------
static const void* LoadContent(unsigned int id, unsigned int type, size_t* size)
{
  HMODULE module = GetModuleHandle(NULL);
  HRSRC rc = FindResource(module, MAKEINTRESOURCE(id), MAKEINTRESOURCE(type));
  HGLOBAL handle = LoadResource(module, rc);
  if (NULL != size)
  {
    *size = SizeofResource(module, rc);
  }
  return (const void*)LockResource(handle);
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
static GLuint CreateShader()
{
  GLuint shader = glCreateProgram();

  std::vector<GLuint> stages;
  stages.push_back(CompileShader((const char*)LoadContent(IDR_TEST_VS, TEXTFILE, NULL), GL_VERTEX_SHADER));
  stages.push_back(CompileShader((const char*)LoadContent(IDR_TEST_FS, TEXTFILE, NULL), GL_FRAGMENT_SHADER));

  if (!LinkShader(shader, stages))
  {
    glDeleteProgram(shader);
    shader = 0;
  }

  std::for_each(stages.begin(), stages.end(), glDeleteShader);

  return shader;
}

//----------------------------------------------------------

int main(int argc, char* argv[])
{
  Device device;
  device.Init();

  // Create a vertex array object that knows about the vertex and index buffers...
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  GLuint vb = CreateVertexBuffer(NumVertices, sizeof(VertexPosition), Vertices);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  for (size_t i = 0; i < VertexPosition::NumElements; ++i)
  {
    glEnableVertexAttribArray(VertexPosition::VertexDecl[i].Usage);
    glVertexAttribPointer(
      VertexPosition::VertexDecl[i].Usage,
      VertexPosition::VertexDecl[i].NumComponents,
      VertexPosition::VertexDecl[i].BaseType,
      GL_FALSE,
      VertexPosition::Stride,
      (const void*)VertexPosition::VertexDecl[i].Offset);
  }
  GLuint ib = CreateIndexBuffer(sizeof(Indices), Indices);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
  glBindVertexArray(0);

  GLuint shader = CreateShader();
  glUseProgram(shader);
  
  // Get each active shader uniform...
  ShaderParam projectionParam(shader, "Projection");
  ShaderParam viewParam(shader, "View");
  ShaderParam worldParam(shader, "World");
  glUseProgram(0);

  glm::mat4 view = glm::lookAt(glm::vec3(0,0,5), glm::vec3(0), glm::vec3(0,1,0));
  glm::mat4 projection = glm::perspective(45.0f, (float)device.Width/(float)device.Height, 0.1f, 100.0f);
  glm::mat4 world = glm::mat4(1);

  glUseProgram(shader);
  glUniformMatrix4fv(viewParam.Location, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projectionParam.Location, 1, GL_FALSE, glm::value_ptr(projection));
  glUseProgram(0);
  
  float angle = 0;
  bool stopProgram = false;
  while (!stopProgram)
  {
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, glm::normalize(glm::vec3(0.75f, 0.5f, 1)));
    angle += 0.5f;

    glUseProgram(shader);
    glUniformMatrix4fv(worldParam.Location, 1, GL_FALSE, glm::value_ptr(world * rot));
    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_SHORT, (const void*)0);

    glBindVertexArray(0);
    glUseProgram(0);

    SDL_GL_SwapBuffers();
    SDL_Delay(2);
    EventHandler(stopProgram);

  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &ib);
  glDeleteBuffers(1, &vb);
  glDeleteProgram(shader);

  return 0;
}
