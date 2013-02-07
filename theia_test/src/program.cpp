
#include <vector>
#include <theia/program.h>
#include <theia/application.h>
#include <theia/graphics/buffers/vertex_buffer.h>

using namespace theia;
using namespace theia::graphics;


#pragma pack(push, 1)
struct Vertex
{
  Vertex(const glm::vec3& p, const glm::vec4& c)
    : pos(p), colour(c)
  {
  }

  glm::vec3 pos;
  glm::vec4 colour;

  static const std::vector<VertexElement>& GetElements();
};
#pragma pack(pop)

const std::vector<VertexElement>& Vertex::GetElements()
{
  static const VertexElement elements[] =
  {
    VertexElement(VertexElementType::Vector3, offsetof(Vertex, pos)),
    VertexElement(VertexElementType::Colour, offsetof(Vertex, colour))
  };
  static const size_t numElements = sizeof(elements)/sizeof(elements[0]);
  static std::vector<VertexElement> list(elements, elements + numElements);

  return list;
}

static const Vertex Vertices[] =
{
  Vertex(glm::vec3( 0,      0.75f, 0), glm::vec4(1, 0, 0, 1)),
  Vertex(glm::vec3(-0.75f, -0.75f, 0), glm::vec4(0, 1, 0, 1)),
  Vertex(glm::vec3( 0.75f, -0.75f, 0), glm::vec4(0, 0, 1, 1))
};
const size_t NumVertices = sizeof(Vertices)/sizeof(Vertices[0]);


class MyApp : public theia::Application
{
public:
  theia::graphics::ClearState clearState;
  VertexBuffer* vb;

  MyApp()
  {
  }
  
  virtual ~MyApp()
  {
  }

  virtual void Initialise()
  {
    Device.SetBackbufferWidth(800);
    Device.SetBackbufferHeight(600);

    clearState.colour = glm::vec4(0, 0, 1, 1);

    vb = VertexBuffer::Create(Vertex::GetElements(), NumVertices);

    Application::Initialise();
  }

  virtual void Exiting()
  {
    delete vb;
    Application::Exiting();
  }

  virtual void Update(unsigned int elapsedMS)
  {
    if (Keyboard.IsKeyDown(SDLK_ESCAPE))
    {
      Stop();
    }

    if (Keyboard.IsKeyDown(SDLK_SPACE) && Keyboard.WasKeyUp(SDLK_SPACE))
    {
      clearState.colour = glm::vec4(1, 0, 0, 1);
    }
    else if (Keyboard.IsKeyUp(SDLK_SPACE) && Keyboard.WasKeyDown(SDLK_SPACE))
    {
      clearState.colour = glm::vec4(0, 0, 1, 1);
    }

    Application::Update(elapsedMS);
  }

  virtual void Render(unsigned int elapsedMS)
  {
    Device.Clear(clearState);
    Application::Render(elapsedMS);
  }
};

void theia::Program::Start()
{
  MyApp app;

  app.Start();
}
