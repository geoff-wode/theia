#include <stdlib.h>

#include <core/game.h>
#include <renderer/graphics_device.h>
#include <renderer/gl_loader.h>
#include <SDL.h>

struct MyGame : public theia::core::Game
{
  MyGame() { }
  virtual ~MyGame() { }
  
  virtual void LoadContent()
  {
    //glClearColor(1, 0, 0, 1);
    Game::LoadContent();
  }

  virtual void Render(uint32_t elapsed)
  {
    static bool white = true;
    static uint32_t delay = 0;

    glClear(GL_COLOR_BUFFER_BIT);

    const uint32_t Period = 3000;
    delay += elapsed;
    if (delay > Period)
    {
      delay -= Period;

      if (white)
      {
        glClearColor(0,0,1,1);
        white = false;
      }
      else
      {
        glClearColor(1,0,0,1);
        white = true;
      }
    }

    Game::Render(elapsed);
  }
};

int main(int argc, char* argv[])
{
  MyGame game;

  theia::renderer::GraphicsDevice::Initialise();

  game.Start();

  return 0;
}
