#include <memory>
#include <glimg/glimg.h>
#include <utils/maths.h>
#include <renderer/texture2D.h>
#include <renderer/gl_loader.h>

Texture2D::Texture2D(
    uint32_t width,
    uint32_t height,
    bool mipmap,
    GLint format)
  : Width(width), Height(height), UseMipMap(mipmap),
    InternalFormat(format)
{
  // Get the number of mipmap levels by computing the integer log base 2 of largest
  // of width and height.
  const uint32_t max = (Width > Height) ? Width : Height;
  const uint32_t levels = UseMipMap ? theia::Log2(max) : 0;

  glGenTextures(1, &texture);
}

Texture2D::~Texture2D()
{
}

bool Texture2D::Load(const std::string& filename)
{
  bool okay = false;

  glBindTexture(GL_TEXTURE_2D, texture);

  return okay;
}
