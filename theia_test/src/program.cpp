#include <stdint.h>
#include <boost/scoped_ptr.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <theia/program.h>
#include <theia/application.h>
#include <theia/graphics/vertex_position.h>
#include <theia/graphics/buffers/vertex_buffer.h>
#include <theia/graphics/buffers/index_buffer.h>
#include <theia/graphics/device.h>

using namespace theia;
using namespace theia::graphics;

static VertexPosition Vertices[] =
{
  VertexPosition(glm::vec3(-1, 1, 1)), // , glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),    // 0
  VertexPosition(glm::vec3(-1,-1, 1)), // , glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),    // 1
  VertexPosition(glm::vec3( 1,-1, 1)), // , glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)),    // 2
  VertexPosition(glm::vec3( 1, 1, 1)), // , glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)),    // 3
  VertexPosition(glm::vec3( 1, 1,-1)), // , glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)),    // 4
  VertexPosition(glm::vec3( 1,-1,-1)), // , glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)),    // 5
  VertexPosition(glm::vec3(-1,-1,-1)), // , glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),    // 6
  VertexPosition(glm::vec3(-1, 1,-1)) // , glm::vec4(0.5f, 0.5f, 0.5f, 1.0f))     // 7
};
static const size_t NumVertices = sizeof(Vertices)/sizeof(Vertices[0]);

static const uint16_t Indices[] =
{
  0, 1, 2,    2, 3, 0,    // Front
  3, 2, 5,    5, 4, 3,    // Right
  4, 5, 6,    6, 7, 4,    // Back
  7, 6, 1,    1, 0, 7,    // Left
  0, 3, 4,    4, 7, 0,    // Top
  1, 6, 5,    5, 2, 1     // Bottom
};
static const size_t NumIndices = sizeof(Indices)/sizeof(Indices[0]);

class MyApp : public theia::Application
{
public:
  ClearContext    clearContext;
  VertexBufferPtr vb;
  IndexBufferPtr  ib;

  MyApp()
  {
    Device.Width = 800;
    Device.Height = 600;
  }
  
  virtual ~MyApp()
  {
  }

  virtual void Initialise()
  {
    clearContext.Colour = glm::vec4(1, 0, 0, 1);

    vb = VertexBuffer::Create(NumVertices, VertexPosition::Declaration);
    vb->SetData(NumVertices, 0, Vertices);
    ib = IndexBuffer::Create(NumIndices, IndexBufferDataType::UInt16);
    ib->SetData(NumIndices, 0, Indices);

    Application::Initialise();
  }

  virtual void Exiting()
  {
    Application::Exiting();
  }

  virtual void Update(unsigned int elapsedMS)
  {
    if (Keyboard.IsKeyDown(SDLK_ESCAPE))
    {
      Stop();
    }

    Application::Update(elapsedMS);
  }

  virtual void Render(unsigned int elapsedMS)
  {
    Device.Clear(clearContext);
    Application::Render(elapsedMS);
  }
};

void theia::Program::Start()
{
  MyApp app;

  app.Start();
}
