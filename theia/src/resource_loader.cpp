#include <SDL.h>
#include <Windows.h>
#include <theia/resource_loader.h>

void theia::ResourceLoader::Load(uint32_t id, uint32_t type, Resource& resource)
{
  resource.data = NULL;
  resource.sizeInBytes = 0;

  HMODULE module = GetModuleHandle(NULL);
  HRSRC rc = FindResource(module, MAKEINTRESOURCE(id), MAKEINTRESOURCE(type));
  HGLOBAL handle = LoadResource(module, rc);
  if (NULL != handle)
  {
    resource.sizeInBytes = SizeofResource(module, rc);
    resource.data = (void*)LockResource(handle);
  }
}
