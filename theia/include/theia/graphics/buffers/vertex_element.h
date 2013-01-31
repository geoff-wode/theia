/// Vertex formats define how data in a buffer maps to the vertex shader attribute inputs at runtime.
///
/// Vertex formats contain information about the type and size of an individual data element within the
/// a single vertex and provide a means to associate a buffer containing vertex data with the format.
///
/// During rendering, the vertex format(s) in the associated render call are used to define the binding between
/// the input data and the active shader. The same vertex format can be used for different vertex data buffers
/// by modifying the buffer association.
///
/// Several pre-defined attribute associations are available to vertex shaders, which should use them to declare
/// the vertex attribute inputs as follows:
/// @code
///   POSITION0 in vec3 Position;
///   TEXCOORD0 in vec2 TexCoord;
/// @endcode
///
/// The pre-defined associations are composed of a usage type and an index value.
///
/// Each index specifies which data stream (aka vertex buffer) the attribute data comes from.
/// It is legal to specify non-sequential indexes, as in
///
/// @code
///   POSITION0 in vec3 Position;
///   TEXCOORD1 in vec2 TexCoord;
/// @endcode
///
/// but the indices in the corresponding @see VertexBuffer creation must match:
///
/// @code
///   const VertexElement Vertex::Elements[] =
///   {
///     VertexElement(VertexElementType::Vector3, VertexElementUsage::Position, 0),
///     VertexElement(VertexElementType::Vector2, VertexElementUsage::TexCoord, 1)
///   };
/// @endcode
///
/// @note All vertex input to a shader <b>must</b> include at least a POSITION0 attribute.

#if ! defined(__THEIA_GFX_VERTEX_ELEMENT__)
#define __THEIA_GFX_VERTEX_ELEMENT__

#include <vector>

namespace theia
{
  namespace graphics
  {
    namespace VertexElementType
    {
      /// Each vertex element must be one of these formats.
      /// @note @see Colour is provided as a convenience.
      enum Enum
      {
        Float,            //< 32-bit floating point value.
        Vector2,          //< Two-component vector of 32-bit floating point values.
        Vector3,          //< Three-component vector of 32-bit floating point values.
        Vector4,          //< Four-component vector of 32-bit floating point values.
        HalfVector2,      //< Two-component vector of 16-bit floating point values.
        HalfVector3,      //< Three-component vector of 16-bit floating point values.
        HalfVector4,      //< Four-component vector of 16-bit floating point values.
        Colour            //< Convenience format to distinguish colours from generic @see Vector4 formats.
      };

      size_t Size(VertexElementType::Enum type);
    }

    namespace VertexElementUsage
    {
      /// Specifies how the element matches to a corresponding shader attribute.
      /// @note For general-purpose values, use a TexCoord.
      enum Enum
      {
        Position,         //< Associates to a POSITION<i>n</i> vertex shader attribute.
        Normal,           //< Associates to a NORMAL<i>n</i> vertex shader attribute.
        Colour,           //< Associates to a COLOUR<i>n</i> vertex shader attribute.
        TexCooord         //< Associates to a TEXCOORD<i>n</i> vertex shader attribute.
      };
    }

    /// Defines one element within a vertex structure.
    struct VertexElement
    {
      /// Define the structure of one vertex element.
      ///
      /// @param[in] type     The data type of the element.
      /// @param[in] usgae    The way in which the element will be used.
      /// @param[in] index    For a given usage, specifies the zero-based "instance" index of this element. The index
      ///                     parameter allows for vertex data to be specified in multiple buffers, the index acting as
      ///                     a reference to each buffer.
      /// @param[in] offset   Offset in bytes from the start of the vertex data to this element. This can be computed
      ///                     using the offsetof() macro defined in stddef.h.
      VertexElement(VertexElementType::Enum type, VertexElementUsage::Enum usage, unsigned int index, size_t offset);

      VertexElement(const VertexElement& other);

      VertexElementType::Enum Type() const { return type; }
      VertexElementUsage::Enum Usage() const { return usage; }
      unsigned int Index() const { return index; }
      size_t Offset() const { return offset; }

    private:
      VertexElementType::Enum type;
      VertexElementUsage::Enum usage;
      unsigned int index;
      size_t offset;
    };
    
    typedef std::vector<VertexElement>  VertexElementList;

  }
}

#endif // __THEIA_GFX_VERTEX_ELEMENT__
