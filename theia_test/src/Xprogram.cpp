#include <stdint.h>
#include <boost/scoped_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <theia/program.h>
#include <theia/application.h>
#include <theia/graphics/vertex_position_colour.h>
#include <theia/graphics/mesh.h>
#include <theia/graphics/shaders/shader.h>
#include <theia/graphics/device.h>
#include <theia/resource_loader.h>

#include "../resources.h"

using namespace theia;
using namespace theia::graphics;

static VertexPositionColour Vertices[] =
{
  VertexPositionColour(glm::vec3(-0.5f, -0.5f, 0), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),    // 0
  VertexPositionColour(glm::vec3( 0.5f, -0.5f, 0), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),    // 0
  VertexPositionColour(glm::vec3( 0.0f,  0.5f, 0), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),    // 0
  //VertexPositionColour(glm::vec3(-1, 1, 1), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),    // 0
  //VertexPositionColour(glm::vec3(-1,-1, 1), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),    // 1
  //VertexPositionColour(glm::vec3( 1,-1, 1), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)),    // 2
  //VertexPositionColour(glm::vec3( 1, 1, 1), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)),    // 3
  //VertexPositionColour(glm::vec3( 1, 1,-1), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)),    // 4
  //VertexPositionColour(glm::vec3( 1,-1,-1), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)),    // 5
  //VertexPositionColour(glm::vec3(-1,-1,-1), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),    // 6
  //VertexPositionColour(glm::vec3(-1, 1,-1), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f))     // 7
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
  RenderContext   renderContext;
  glm::mat4 projection;
  glm::mat4 view;
  float angle;

  MyApp()
  {
    angle = 0.0f;
  }
  
  virtual ~MyApp()
  {
  }

  virtual void Initialise()
  {
    theia::graphics::DeviceState state;
    state.IsFullScreen = false;
    state.ScreenWidth = 800;
    state.ScreenHeight = 600;
    theia::graphics::InitialiseDevice(state);

    //clearContext.Colour = glm::vec4(0, 0, 0, 1);

    renderContext.VertexBuffer = VertexBuffer::Create(NumVertices, VertexPositionColour::Declaration);
    renderContext.VertexBuffer->SetData(NumVertices, 0, Vertices);

    //renderContext.indexBuffer = IndexBuffer::Create(Indices, NumIndices);

    theia::Resource vs;
    theia::LoadResource(IDR_TEST_VS, TEXTFILE, vs);
    theia::Resource fs;
    theia::LoadResource(IDR_TEST_FS, TEXTFILE, fs);
    renderContext.Shader = Shader::Create((const char*)vs.Data, (const char*)fs.Data);

    const float aspectRatio = (float)state.ScreenWidth / (float)state.ScreenHeight;
    projection = glm::perspective(45.0f, aspectRatio, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(0,0,10), glm::vec3(0), glm::vec3(0,1,0));

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
    theia::graphics::ClearScreen(clearContext);
    theia::graphics::Render(1, renderContext);

    Application::Render(elapsedMS);
  }
};

void theia::Program::Start()
{
  MyApp app;

  app.Start();
}
