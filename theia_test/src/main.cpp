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

static MeshPtr mesh(NULL);
static Shader::Parameter* worldParam(NULL);

static const glm::mat4 MatrixIdentity(1);
static const glm::vec3 Up(0,1,0);

//----------------------------------------------

extern void InitSystem();

struct Material
{
  Material(ShaderPtr shader)
    : Ke(0), Ka(0.2f), Kd(0.7f),
      shader(shader),
      emissiveParam(shader->GetParameter("Ke")),
      ambientParam(shader->GetParameter("Ka")),
      diffuseParam(shader->GetParameter("Kd"))
  { }

  void Apply() const
  {
    shader->SetParameter(emissiveParam, Ke);
    shader->SetParameter(ambientParam, Ka);
    shader->SetParameter(diffuseParam, Kd);
  }

  glm::vec3 Ke;
  glm::vec3 Ka;
  glm::vec3 Kd;

  ShaderPtr shader;
  Shader::Parameter* emissiveParam;
  Shader::Parameter* ambientParam;
  Shader::Parameter* diffuseParam;
};

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

static void Update()
{
  const unsigned int frameRate = 1000 / 60;
  static float simTime = 0.0f;
  static float previousTime = 0.0f;
  static float angle = 0.0f;

  static const glm::mat4 scale = glm::scale(MatrixIdentity, glm::vec3(0.5f));

  float now = (float)SDL_GetTicks();
  float deltaMS = (now - previousTime) / 1000.0f;
  if (deltaMS > frameRate) { deltaMS = frameRate; }
  previousTime = now;
  angle += 60 * deltaMS;

  glm::mat4 rotation = glm::rotate(MatrixIdentity, angle, Up);
  glm::mat4 world = scale * rotation;
  mesh->GetShader()->SetParameter(worldParam, world);
  glClear(GL_COLOR_BUFFER_BIT);
  mesh->Render();
}

//----------------------------------------------

int main(int argc, char* argv[])
{
  LOG("----\n");
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

  worldParam = shader->GetParameter("World");
  Shader::Parameter* viewProjParam = shader->GetParameter("ViewProjection");

  shader->SetParameter(shader->GetParameter("LightPosition"), glm::vec3(5,5,10));
  shader->SetParameter(shader->GetParameter("LightColour"), glm::vec3(1));
  shader->SetParameter(shader->GetParameter("AmbientLightColour"), glm::vec3(0.2f));

  Material material(shader);
  material.Apply();

  glm::mat4 view(glm::lookAt(glm::vec3(0,2,5), glm::vec3(0), glm::vec3(0,1,0)));
  glm::mat4 projection(glm::perspective(45.0f, 800.0f/600.0f, 0.1f, 100.0f));

  shader->SetParameter(viewProjParam, projection * view);

  mesh = Mesh::Factory::New();
  mesh->SetVertexBuffer(vb);
  mesh->SetIndexBuffer(ib);
  mesh->SetShader(shader);

  glClearColor(0.5f, 0.5f, 0.5f, 1);
  bool quit = false;
  while (!quit)
  {
    Update();

    SDL_GL_SwapBuffers();
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT: quit = true; break;
      case SDL_KEYDOWN: quit = (event.key.keysym.sym == SDLK_ESCAPE); break;
      default: break;
      }
    }
  }

  return 0;
}
