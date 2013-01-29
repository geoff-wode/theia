
#include <theia/program.h>
#include <theia/application.h>

using namespace theia;

class MyApp : public theia::Application
{
public:
  theia::graphics::ClearState clearState;

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

    Application::Initialise();
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
    Device.Clear(clearState);
    Application::Render(elapsedMS);
  }
};

void theia::Program::Start()
{
  MyApp app;

  app.Start();
}
