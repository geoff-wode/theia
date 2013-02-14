#include <glm/glm.hpp>
#include <SDL.h>
#include <vector>
#include <stdint.h>
#include <stddef.h>
#include <shader.h>
#include <debug.h>
#include <gl_loader.h>
#include <vertex_decl.h>
#include <mesh.h>
#include "../resources.h"

//----------------------------------------------

extern void InitSystem();

//----------------------------------------------

struct Vertex
{
  glm::vec3 pos;
  glm::vec4 colour;
};

static const VertexElement elements[] =
{
  { GL_FLOAT, 3, offsetof(Vertex, pos) },
  { GL_FLOAT, 4, offsetof(Vertex, colour) }
};

const VertexDeclaration VertexDecl =
{
  sizeof(Vertex),
  sizeof(elements)/sizeof(elements[0]),
  elements
};

//----------------------------------------------

static Vertex vertices[] =
{
  { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(1,0,0,1) },
  { glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec4(0,1,0,1) },
  { glm::vec3( 0.0f,  0.5f, 0.0f), glm::vec4(0,0,1,1) }
};
const size_t NumVertices = sizeof(vertices)/sizeof(vertices[0]);

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
  InitSystem();

  VertexBufferPtr vb = VertexBuffer::Factory::New();
  vb->Configure(&VertexDecl, NumVertices);
  vb->SetData(NumVertices, 0, vertices);

  ShaderPtr shader = Shader::Factory::New();
  shader->Compile(
    (const char* const)LoadResource(IDR_TEST_VS, TEXTFILE),
    (const char* const)LoadResource(IDR_TEST_FS, TEXTFILE));

  MeshPtr mesh = Mesh::Factory::New();
  mesh->SetVertexBuffer(vb);
  mesh->SetShader(shader);

  Shader::Parameter* durationParam = shader->GetParameter("duration");
  Shader::Parameter* timeParam = shader->GetParameter("time");

  shader->SetParameter(durationParam, 5.0f);

  bool quit = false;
  while (!quit)
  {
    glClear(GL_COLOR_BUFFER_BIT);

    float time = (float)SDL_GetTicks() / 1000.0f;

    shader->SetParameter(timeParam, time);
    mesh->Render();

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
