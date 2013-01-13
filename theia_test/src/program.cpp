#include <stdlib.h>
#include <core/game.h>
#include <core/keyboard.h>
#include <renderer/graphics_device.h>

using namespace theia;
using namespace theia::renderer;

class MyGame : public theia::Game
{
public:
  MyGame() { }
  virtual ~MyGame() { }

  virtual void Initialise()
  {
    Game::Initialise();

    Device = new GraphicsDevice();
    Device->Initialise();
  }
  
  virtual void DeviceReady()
  {
    Game::DeviceReady();
  }

  virtual void Update(uint32_t elapsed)
  {
    if (Keyboard::IsKeyDown(SDLK_ESCAPE))
    {
      Stop();
    }
  }

  virtual void Render(uint32_t elapsed)
  {
    Device->Clear(Colour::TransparentBlack);
    Game::Render(elapsed);
  }
};

int Program::Run(int argc, char* argv[])
{
  MyGame game;

  game.Start();

  return 0;
}
