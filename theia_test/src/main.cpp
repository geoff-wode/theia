#include <glm/glm.hpp>
#include <SDL.h>
#include <vector>
#include <stdint.h>
#include <stddef.h>
#include <shader.h>
#include <debug.h>
#include <gl_loader.h>
#include "../resources.h"

//----------------------------------------------

struct VertexElement
{
  VertexElement(GLenum type, size_t components, size_t offset)
    : type(type), components(components), offset(offset)
  { }
  GLenum  type;
  size_t  components;
  size_t  offset;
};

struct VertexDeclaration
{
  size_t  stride;
  size_t  numElements;
  const VertexElement* const elements;

  void Configure() const
  {
    for (size_t i = 0; i < numElements; ++i)
    {
      glEnableVertexAttribArray(i);
      glVertexAttribPointer(i, elements[i].components, elements[i].type, GL_FALSE, stride, (const void*)elements[i].offset);
    }
  }
};

//----------------------------------------------

struct Mesh
{
  Mesh(const VertexDeclaration* const vertexDecl, size_t numVertices)
    : vertexDecl(vertexDecl)
  {
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numVertices * vertexDecl->stride, NULL, GL_STATIC_DRAW);
    vertexDecl->Configure();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  
  ~Mesh() { glDeleteBuffers(1, &vertexBuffer); }

  void SetData(size_t size, size_t offset, const void* data)
  { 
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void Render(size_t numPrimitives) const
  {
    Shaders::Apply(shader);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glDrawArrays(GL_TRIANGLES, 0, 3 * numPrimitives);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
  }

  GLuint  vertexBuffer;
  Shader* shader;
  const VertexDeclaration* const vertexDecl;
};

//----------------------------------------------

struct Vertex
{
  Vertex(const glm::vec3& p, const glm::vec4&c) : pos(p), colour(c)
  { }

  glm::vec3 pos;
  glm::vec4 colour;

  static const VertexDeclaration VertexDecl;
};

static const VertexElement elements[] =
{
  VertexElement(GL_FLOAT, 3, offsetof(Vertex, pos)),
  VertexElement(GL_FLOAT, 4, offsetof(Vertex, colour))
};

const VertexDeclaration Vertex::VertexDecl =
{
  sizeof(Vertex), sizeof(elements)/sizeof(elements[0]), elements
};

//----------------------------------------------

static Vertex vertices[] =
{
  Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(1,0,0,1)),
  Vertex(glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec4(0,1,0,1)),
  Vertex(glm::vec3( 0.0f,  0.5f, 0.0f), glm::vec4(0,0,1,1))
};

//----------------------------------------------

static void ConfigureSDL()
{
  static const struct SDLAttribute
  {
    const char* name;
    SDL_GLattr  attr;
    int         value;
  } sdlAttributes[] =
  {
    { "SDL_GL_DOUBLEBUFFER",  SDL_GL_DOUBLEBUFFER,            1 },
    { "SDL_GL_RED_SIZE",      SDL_GL_RED_SIZE,                8 },
    { "SDL_GL_GREEN_SIZE",    SDL_GL_GREEN_SIZE,              8 },
    { "SDL_GL_BLUE_SIZE",     SDL_GL_BLUE_SIZE,               8 },
    { "SDL_GL_ALPHA_SIZE",    SDL_GL_ALPHA_SIZE,              8 },
    { "SDL_GL_BUFFER_SIZE",   SDL_GL_BUFFER_SIZE,             24 },
    { "SDL_GL_MULTISAMPLEBUFFERS", SDL_GL_MULTISAMPLEBUFFERS, 1 },
    { "SDL_GL_MULTISAMPLESAMPLES", SDL_GL_MULTISAMPLESAMPLES, 4 }
  };
  const int NumAttributes = sizeof(sdlAttributes)/sizeof(sdlAttributes[0]);
  for (int i = 0; i < NumAttributes; ++i)
  {
    SDL_GL_SetAttribute(sdlAttributes[i].attr, sdlAttributes[i].value);
  }
}

//----------------------------------------------

static void ConfigureGL()
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
}

//----------------------------------------------

static const void* LoadResource(uint32_t id, uint32_t type)
{
  HMODULE module = GetModuleHandle(NULL);
  HRSRC rc = FindResource(module, MAKEINTRESOURCE(id), MAKEINTRESOURCE(type));
  HGLOBAL handle = LoadResource(module, rc);
  //if (NULL != size)
  //{
  //  *size = SizeofResource(module, rc);
  //}
  return (const void*)LockResource(handle);
}

//----------------------------------------------

int main(int argc, char* argv[])
{
  SDL_Init(SDL_INIT_EVERYTHING);
  atexit(SDL_Quit);

  ConfigureSDL();
  if (NULL == SDL_SetVideoMode(800, 600, 32, SDL_OPENGL | SDL_HWSURFACE))
  {
    exit(EXIT_FAILURE);
  }
  ogl_LoadFunctions();
  ConfigureSDL();

  Mesh mesh(&Vertex::VertexDecl, sizeof(vertices)/sizeof(vertices[0]));
  mesh.SetData(sizeof(vertices), 0, vertices);
  mesh.shader = Shaders::Create(
    (const char* const)LoadResource(IDR_TEST_VS, TEXTFILE),
    (const char* const)LoadResource(IDR_TEST_FS, TEXTFILE));

  Shaders::Parameter offsetParam = Shaders::GetParameter(mesh.shader, "offset");

  bool quit = false;
  while (!quit)
  {
    glClear(GL_COLOR_BUFFER_BIT);

    Shaders::SetParameter(mesh.shader, offsetParam, glm::vec2(0.5f, -0.25f));
    mesh.Render(1);

    SDL_GL_SwapBuffers();
    SDL_Delay(2);

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT: quit = true; break;
      default: break;
      }
    }
  }

  return 0;
}
