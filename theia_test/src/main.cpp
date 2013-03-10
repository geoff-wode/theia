#define GLM_SWIZZLE
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
const int screenWidth = 1280;
const int screenHeight = 720;
const float halfFOV = 45.0f;
const float aspectRatio = (float)screenWidth / (float)screenHeight;
const int gridSize = 256;
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

// set the Radius of the sphere...
const float Radius = 6300;

// place the sphere...
const glm::vec3 Target(500000,0,500000);

// set the eye position to some multiple of the Radius (so we can see the damn thing)...
const glm::vec3 EyePos = Target + (glm::vec3(0,0.0f,-1) * Radius * 3.0f);

// try to minimise the distance between the near and far bounding planes:
// near must be closer than the sphere while far bounding plane must be at least (near + sphere_location)...
const float NearPlane = Radius * 1.1f;
const float FarPlane = glm::distance(EyePos, Target) + NearPlane;

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

//----------------------------------------------

extern void InitSystem(int screenWidth, int screenHeight);

//----------------------------------------------

static void BuildGrid(const glm::vec3& X, const glm::vec3& Y, int gridSize, Vertex* vertices)
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
      vertices[i].position = Radius * glm::normalize(P + ((float)x * stepX) + (0.5f * normal));
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
  InitSystem(1280,720);

  theia::ShaderPtr shader(new theia::Shader());
  shader->Compile(IDR_SHADER_COMMON, IDR_TEST_VS, IDR_TEST_FS);

  theia::MaterialState material(shader);
  theia::Material::Apply(material);

  // create one vertex buffer with all the vertices for all 6 faces of the cube...
  theia::VertexBufferPtr sphereVertices;
  {
    std::vector<Vertex> vertices(gridSize * gridSize * 6);
    for (int face = 0; face < 6; ++face)
    {
      const int offset = gridSize * gridSize;
      BuildGrid(tangents[face].x, tangents[face].y, gridSize, vertices.data() + (offset * face));
    }
    sphereVertices = theia::VertexBuffer::Create(vertices.size() * sizeof(Vertex));
    sphereVertices->SetData(vertices.size() * sizeof(Vertex), 0, vertices.data());
  }

  // Create one index buffer that defines a triangle strip for just a single face of the cube.
  // An instanced render call is used later to run this triangle strip 6 times over the vertex
  // buffer...
  const int numIndices = gridSize * 2 * (gridSize-1);
  theia::IndexBufferPtr ib = theia::IndexBuffer::Create(numIndices * sizeof(unsigned short));
  {
    std::vector<unsigned short> indices(numIndices);
    BuildIndices(gridSize, indices);
    ib->SetData(indices.size() * sizeof(unsigned short), 0, indices.data());
  }

  // Create an object to hold all the buffer state in a single place...
  GLuint vao;
  {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->buffer);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVertices->buffer);
    Vertex::Configure();
    glBindVertexArray(0);
    // VAO is now configured so unset the GL global buffer bind state...
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  glm::mat4 projection(glm::perspective(halfFOV, aspectRatio, NearPlane, FarPlane));
  
  shader->SetParameter(shader->GetParameter("AmbientLight"), glm::vec3(0.2f));
  shader->SetParameter(shader->GetParameter("GridLineWidth"), glm::vec2(1));
  shader->SetParameter(shader->GetParameter("GridResolution"), glm::vec2(1.0f / 20.0f, 1.0f / 10.0f));

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
    angle += 20 * deltaMS;

    const glm::mat4 translation(glm::translate(MatrixIdentity, Target));
    const glm::mat4 tilt(glm::rotate(MatrixIdentity, 20.0f, glm::vec3(0,0,1)));
    glm::mat4 rotation(glm::rotate(MatrixIdentity, angle, Up));
    glm::mat4 model(translation * tilt * rotation);

    // Combine the view and model transform matrices and translate the result to make objects
    // relative to the eye before making the final screen-space perspective transform...
    glm::mat4 view(glm::lookAt(EyePos, Target, Up));
    glm::mat4 mv(glm::translate(view * model, -EyePos));
    glm::mat4 mvp(projection * mv);

    shader->SetParameter(shader->GetParameter("EyePosition"), EyePos);
    shader->SetParameter(shader->GetParameter("World"), model);
    shader->SetParameter(shader->GetParameter("WorldViewProjection"), mvp);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->Activate();

    glBindVertexArray(vao);
    // Render the vertices as 6 instances of indexed triangle strips...
    for (int i = 0; i < 6; ++i)
    {
      // See http://stackoverflow.com/questions/9431923/using-an-offset-with-vbos-in-opengl/9434876#9434876
      // for a quick summary...
      glDrawElementsBaseVertex(
        GL_TRIANGLE_STRIP,        // what kind of thing to render
        numIndices,               // how many elements (_NOT_ primitives!) to render
        GL_UNSIGNED_SHORT,        // index type
        (const void*)0,           // offset from start of index buffer
        gridSize * gridSize * i); // offset to add to each index
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
