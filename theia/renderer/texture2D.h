#ifndef __TEXTURE__
#define __TEXTURE__

#include <string>
#include <stdint.h>
#include <stdlib.h>
#include <renderer/gl_loader.h>
#include <utils/property.h>

class Texture2D
{
public:
  Texture2D(
    uint32_t width,
    uint32_t height,
    bool mipmap,
    GLint internalFormat);

  virtual ~Texture2D();

  bool Load(const std::string& filename);
  void Save(const std::string& filename);

  void SetData(const uint8_t* pixels);

  Property<uint32_t> Width;
  Property<uint32_t> Height;
  Property<GLenum> InternalFormat;
  Property<GLenum> ExternalFormat;
  Property<GLenum> ExternalFormatType;
  Property<bool> UseMipMap;

protected:
  GLuint texture;
};

#endif // __TEXTURE__
