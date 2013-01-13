/// Vertex attributes are shader inputs.

#if ! defined(__THEIA_SHADER_VERTEX_ATTRIB__)
#define __THEIA_SHADER_VERTEX_ATTRIB__

namespace theia
{
  namespace renderer
  {
    namespace VertexAttributeType
    {
      enum e
      {
        Float,
        Vector2,
        Vector3,
        Vector4,
        Matrix2x2,
        Matrix3x3,
        Matrix4x4,
        Int
      };
    }

    struct VertexAttribute
    {
      VertexAttribute(const char* const name, VertexAttributeType::e type)
        : name(name), type(type)
      {
      }

      const char* const   Name() const { return name; }
      const unsigned int  Location() const { return location; }
      const VertexAttributeType::e  Type() const { return type; }

    private:
      const char* name;
      unsigned int location;
      VertexAttributeType::e type;

      friend struct ShaderProgram;
    };
  }
}

#endif // __THEIA_SHADER_VERTEX_ATTRIB__
