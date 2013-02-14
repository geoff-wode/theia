

#if ! defined( __RESOURCE_MANAGER__ )
#define __RESOURCE_MANAGER__

#include <stddef.h>
#include <stdint.h>

namespace ResourceManager
{
  struct Resource
  {
    size_t  sizeInBytes;
    const void* data;
  };

  Resource Load(uint16_t type, uint16_t id);
}

#endif // __RESOURCE_MANAGER__
