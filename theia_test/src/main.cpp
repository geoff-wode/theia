#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
#include "../models/Cube.h"

//----------------------------------------------

extern void InitSystem();

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
  vb->Configure(Cube::VertexDecl, Cube::NumVertices);
  vb->SetData(Cube::NumVertices, 0, Cube::Vertices);

  IndexBufferPtr ib = IndexBuffer::Factory::New();
  ib->Configure(Cube::NumIndices, IndexBufferDataType::UnsignedShort);
  ib->SetData(Cube::NumIndices, 0, Cube::Indices);

  ShaderPtr shader = Shader::Factory::New();
  shader->Compile(
    (const char* const)LoadResource(IDR_TEST_VS, TEXTFILE),
    (const char* const)LoadResource(IDR_TEST_FS, TEXTFILE));

  Shader::Parameter* worldParam = shader->GetParameter("World");
  Shader::Parameter* viewProjParam = shader->GetParameter("ViewProjection");

  glm::mat4 view(glm::lookAt(glm::vec3(0,0,5), glm::vec3(0), glm::vec3(0,1,0)));
  glm::mat4 projection(glm::perspective(45.0f, 800.0f/600.0f, 0.1f, 100.0f));
  glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(0.5f));

  shader->SetParameter(viewProjParam, projection * view);

  MeshPtr mesh = Mesh::Factory::New();
  mesh->SetVertexBuffer(vb);
  mesh->SetIndexBuffer(ib);
  mesh->SetShader(shader);

  float angle = 0;
  bool quit = false;
  while (!quit)
  {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(1));
    glm::mat4 rotation = glm::rotate(glm::mat4(1), angle, glm::vec3(0,1,0));
    glm::mat4 world = scale * rotation;
    shader->SetParameter(worldParam, world);
    angle += 0.5f;

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
