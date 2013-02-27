#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <vector>
#include <stdint.h>
#include <stddef.h>
#include <shader.h>
#include <debug.h>
#include <gl_loader.h>
#include "../resources.h"

//----------------------------------------------
const int gridSize = 128;
//----------------------------------------------

struct Vertex
{
  glm::vec3 position;

  static void Configure()
  {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
  }
};

struct Mesh
{
  GLuint vao;
  GLuint vb;
  size_t primitiveCount;
};

struct Material
{
  Material(ShaderPtr shader)
    : Ke(0),
      Ka(0.1f),
      Kd(0.5f),
      Ks(0.6f,0.6f,0.6f,8),
      shader(shader),
      emissiveParam(shader->GetParameter("Ke")),
      ambientParam(shader->GetParameter("Ka")),
      diffuseParam(shader->GetParameter("Kd")),
      specularParam(shader->GetParameter("Ks"))
  { }

  void Apply() const
  {
    if (emissiveParam) { shader->SetParameter(emissiveParam, Ke); }
    if (ambientParam) { shader->SetParameter(ambientParam, Ka); }
    if (diffuseParam) { shader->SetParameter(diffuseParam, Kd); }
    if (specularParam) { shader->SetParameter(specularParam, Ks); }
  }

  glm::vec3 Ke;
  glm::vec3 Ka;
  glm::vec3 Kd;
  glm::vec4 Ks;

  ShaderPtr shader;
  Shader::Parameter* emissiveParam;
  Shader::Parameter* ambientParam;
  Shader::Parameter* diffuseParam;
  Shader::Parameter* specularParam;
};

//----------------------------------------------

static const glm::mat4 MatrixIdentity(1);

static const glm::vec3 Forward(0,0,-1);
static const glm::vec3 Backward(0,0,1);
static const glm::vec3 Up(0,1,0);
static const glm::vec3 Down(0,-1,0);
static const glm::vec3 Right(1,0,0);
static const glm::vec3 Left(-1,0,0);

//----------------------------------------------

extern void InitSystem();

//----------------------------------------------

static void BuildGrid(const glm::vec3& X, const glm::vec3& Y, int gridSize, std::vector<Vertex>& vertices)
{
  const glm::vec3 normal(glm::normalize(glm::cross(X, Y)));
  const glm::vec3 stepX(X / ((float)gridSize-1));
  const glm::vec3 stepY(Y / ((float)gridSize-1));

  glm::vec3 P((X+Y) * -0.5f);

  int i = 0;
  for (int y = 0; y < gridSize; ++y)
  {
    for (int x = 0; x < gridSize; ++x)
    {
      vertices[i].position = P + ((float)x * stepX) + (0.5f * normal);
      ++i;
    }
    P += stepY;
  }
}

//----------------------------------------------

static void BuildIndices(int gridSize, std::vector<unsigned short>& indices)
{
  // Set indices for a triangle strip...
  int i = 0;
  int z = 0;
  while (z < gridSize - 1)
  {
    for (int x = 0; x < gridSize; ++x)
    {
      indices[i++] = x + (z * gridSize);
      indices[i++] = x + ((z + 1) * gridSize);
    }
    ++z;
    if (z < gridSize - 1)
    {
      for (int x = gridSize - 1; x >= 0; --x)
      {
        indices[i++] = x + ((z + 1) * gridSize);
        indices[i++] = x + (z * gridSize);
      }
    }
    ++z;
  }
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
  LOG("----\n");
  InitSystem();

  ShaderPtr shader = Shader::Factory::New();
  shader->Compile(
    (const char* const)LoadResource(IDR_TEST_VS, TEXTFILE),
    (const char* const)LoadResource(IDR_TEST_FS, TEXTFILE));

  Material material(shader);
  material.Apply();

  Shader::Parameter* worldParam = shader->GetParameter("World");
  Shader::Parameter* viewProjParam = shader->GetParameter("ViewProjection");

  const struct Tangent
  {
    glm::vec3 x;
    glm::vec3 y;
  } tangents[] =
  {
    { glm::vec3(1,0,0), glm::vec3(0,1,0) },
    { glm::vec3(0,0,-1),glm::vec3(0,1,0) },
    { glm::vec3(-1,0,0),glm::vec3(0,1,0) },
    { glm::vec3(0,0,1),glm::vec3(0,1,0) },
    { glm::vec3(1,0,0),glm::vec3(0,0,-1) },
    { glm::vec3(1,0,0),glm::vec3(0,0,1) }
  };
  const int numFaces = sizeof(tangents)/sizeof(tangents[0]);

  std::vector<Vertex> vertices(gridSize * gridSize);

  GLuint vb[numFaces];
  glGenBuffers(numFaces, vb);

  for (int i = 0; i < numFaces; ++i)
  {
    BuildGrid(tangents[i].x, tangents[i].y, gridSize, vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vb[i]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  GLuint ib;
  glGenBuffers(1, &ib);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
  std::vector<unsigned short> indices(gridSize * 2 * (gridSize-1));
  BuildIndices(gridSize, indices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  GLuint vao[numFaces];
  glGenVertexArrays(numFaces, vao);
  for (int i = 0; i < numFaces; ++i)
  {
    glBindVertexArray(vao[i]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBindBuffer(GL_ARRAY_BUFFER, vb[i]);
    Vertex::Configure();
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  //glClearColor(1, 0, 0, 1);
  
  // set the radius of the sphere...
  const float radius = 1500;
  // place the sphere at the world origin...
  const glm::vec3 target(0);
  // set the eye position to some multiple of the radius (so we can see the damn thing)...
  const glm::vec3 eyePos(0,0,radius * 5);
  // try to minimise the distance between the near and far bounding planes:
  // near must be closer than the sphere while far bounding plane must be at least (near + sphere_location)...
  const float nearPlane = radius * 1.1f;
  const float farPlane = glm::distance(eyePos, target) + nearPlane;

  glm::mat4 view(glm::lookAt(eyePos, target, Up));
  glm::mat4 projection(glm::perspective(45.0f, 800.0f/600.0f, nearPlane, farPlane));

  shader->SetParameter(shader->GetParameter("AmbientLightColour"), glm::vec3(1));
  shader->SetParameter(shader->GetParameter("LightPosition"), glm::vec3(radius * 5));
  shader->SetParameter(shader->GetParameter("LightColour"), glm::vec3(1));
  shader->SetParameter(shader->GetParameter("GridLineWidth"), glm::vec2(1));
  shader->SetParameter(shader->GetParameter("GridResolution"), glm::vec2(1.0f / 10.0f));
  shader->SetParameter(shader->GetParameter("Radius"), radius);
  shader->SetParameter(viewProjParam, projection * view);

  const float frameRate = 1000.0f / 60.0f;
  float previousTime = 0.0f;
  float angle = 0.0f;
  bool quit = false;
  while (!quit)
  {
    float now = (float)SDL_GetTicks();
    float deltaMS = (now - previousTime) / 1000.0f;
    if (deltaMS > frameRate) { deltaMS = frameRate; }
    previousTime = now;
    angle -= 60 * deltaMS;

    glm::mat4 rotation = glm::rotate(MatrixIdentity, angle, glm::normalize(glm::vec3(1,1,0)));
    glm::mat4 world = rotation;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->SetParameter(worldParam, world);
    shader->Apply();

    for (int i = 0; i < numFaces; ++i)
    {
      glBindVertexArray(vao[i]);
      glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_SHORT, (const void*)0);
    }

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
