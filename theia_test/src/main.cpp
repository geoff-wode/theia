#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <vector>
#include <stdint.h>
#include <stddef.h>
#include <theia/misc/debug.h>
#include <theia/graphics/shader.h>
#include <theia/graphics/material.h>
#include <theia/graphics/index_buffer.h>
#include <theia/graphics/vertex_buffer.h>
#include <theia/graphics/gl/gl_loader.h>
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

int main(int argc, char* argv[])
{
  LOG("----\n");
  InitSystem();

  theia::ShaderPtr shader(new theia::Shader());
  shader->Compile(IDR_TEST_VS, IDR_TEST_FS);

  theia::MaterialState material(shader);
  theia::Material::Apply(material);

  theia::Shader::Parameter* const worldParam = shader->GetParameter("World");
  theia::Shader::Parameter* const viewProjParam = shader->GetParameter("ViewProjection");

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

  std::vector<theia::VertexBufferPtr> faces(numFaces);

  for (int i = 0; i < numFaces; ++i)
  {
    BuildGrid(tangents[i].x, tangents[i].y, gridSize, vertices);
    faces[i] = theia::VertexBuffer::Create(vertices.size() * sizeof(Vertex));
    faces[i]->SetData(vertices.size() * sizeof(Vertex), 0, vertices.data());
  }

  std::vector<unsigned short> indices(gridSize * 2 * (gridSize-1));
  BuildIndices(gridSize, indices);
  theia::IndexBufferPtr ib = theia::IndexBuffer::Create(indices.size() * sizeof(unsigned short));
  ib->SetData(indices.size() * sizeof(unsigned short), 0, indices.data());

  GLuint vao[numFaces];
  glGenVertexArrays(numFaces, vao);
  for (int i = 0; i < numFaces; ++i)
  {
    glBindVertexArray(vao[i]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->buffer);
    glBindBuffer(GL_ARRAY_BUFFER, faces[i]->buffer);
    Vertex::Configure();
    glBindVertexArray(0);

    // Do this >outside< of the VAO binding..!
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
    shader->Activate();

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
