/**
 Declarations to glue the client code to the Core subsystem.
 */

#if ! defined(__THEIA_GAME__)
#define __THEIA_GAME__

#include <stdint.h>

namespace theia
{
  namespace core
  {
    struct Game
    {
      Game();
      virtual ~Game();

      void Start();

      virtual void Initialise();
      virtual void LoadContent();
      virtual void UnloadContent();
      virtual void Update(uint32_t elapsed);
      virtual void Render(uint32_t elapsed);
    };
  }
}

#endif // __THEIA_GAME__
