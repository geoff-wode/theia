#if ! defined(__RESOURCE_LOADER__)
#define __RESOURCE_LOADER__

#include <stddef.h>
#include <stdint.h>

namespace theia
{
  struct Resource
  {
    size_t  sizeInBytes;
    void*   data;
  };

  namespace ResourceLoader
  {
    void Load(uint32_t id, uint32_t type, Resource& resource);
  }
}

#endif // __RESOURCE_LOADER__
