/**
Vertex elements define the individual "fields" of a vertex structure, with arrays of such structures
held in vertex buffers.

Vertex elements define the type of data and its offset from the start of a vertex in bytes.

There are a maximum of 16 instances of each usage type.
*/

#if ! defined(__THEIA_GFX_VERTEX_ELEMENT__)
#define __THEIA_GFX_VERTEX_ELEMENT__

#include <vector>

namespace theia
{
  namespace graphics
  {
    /// Each vertex element must be one of these formats.
    /// @see Colour is provided as a convenience.
    namespace VertexElementType
    {
      /// Vertex element data type enumeration.
      enum Enum
      {
        Float,            //< 32-bit floating point value.
        Vector2,          //< Two-component vector of 32-bit floating point values.
        Vector3,          //< Three-component vector of 32-bit floating point values.
        Vector4,          //< Four-component vector of 32-bit floating point values.
        HalfVector2,      //< Two-component vector of 16-bit floating point values.
        HalfVector4,      //< Four-component vector of 16-bit floating point values.
        Colour            //< Convenience format to distinguish colours from generic @see Vector4 formats.
      };

      const char* const ToString(VertexElementType::Enum type);
    }

    /// Each vertex element is bound to a usage in the vertex shader via this enumeration.
    /// When using multiple elements with the same usage, the usage index is appended to the corresponding
    /// shader vertex attribute, as in "POSITION0", "POSITION1" etc.
    ///
    /// When only a single element has a particular usage, the index in the shader can be dropped, i.e.
    /// "POSITION" refers to "POSITION0", etc.
    ///
    /// @note TexCoord usage elements must always have the usage index explicitly declared.
    namespace VertexElementUsage
    {
      /// Vertex element usage enumeration.
      enum Enum
      {
        Position,     //< Equates to shader attribute POSITION<i>n</i>.
        Normal,       //< Equates to shader attribute NORMAL<i>n</i>.
        Colour,       //< Equates to shader attribute COLOUR<i>n</i>.
        TexCoord      //< Equates to shader attribute TEXCOORD<i>n</i>.
      };

      const char* const ToString(VertexElementUsage::Enum usage);
    }

    /// Defines one element within a vertex structure.
    struct VertexElement
    {
      /**
      Define the structure of one vertex element.
      
      When vertex data is spread across severeal buffers, the stream parameter determines which of those
      buffers the element is contained within. For example, given 2 vertex data streams called positionsData
      and colourData defined by the structures
      @code
        // Stream 0
        struct VertexPosition           
        {                               
          glm::vec3 pos;                
        };                              
      @endcode
      and
      @code
        // Stream 1
        struct VertexColour
        {
          glm::vec4 colour;
        };
      @endcode
      the VertexElement entries for each would be
      @code
        VertexElement(VertexElementType::Vector3, VertexElementUsage::Position, 0, offsetof(VertexPosition, pos), 0);
      @endcode
      and
      @code
        VertexElement(VertexElementType::Vector4, VertexElementUsage::Colour, 0, offsetof(VertexColour, colour), 1);
      @endcode
      To render this data, the application must assign the two vertex streams to the render state:
      @code
        myRenderState.SetStreams(positionData, colourData);
      @endcode
      Note that the order of the stream data buffers maps directly to the stream value passed to the vertex element
      constructor

      @param[in] type       The data type of the element.
      @param[in] usage      The usage semantic that the element will be used as in a vertex shader.
      @param[in] usageIndex The instance of the usage semantic when several elements share a usage type.
      @param[in] offset     Offset in bytes from the start of the vertex data to this element. This can be computed
                            using the offsetof() macro defined in stddef.h. The maximum offset value is 2047.
      @param[in] stream     The vertex stream from which this element will be taken. The maximum stream index is 15.
      */
      VertexElement(VertexElementType::Enum type,
                    VertexElementUsage::Enum usage, unsigned int usageIndex,
                    size_t offset,
                    unsigned int stream);

      /// Allow copying of vertex elements.
      VertexElement(const VertexElement& other);

      void Enable() const;

      /// The type of the vertex element.
      const VertexElementType::Enum Type;

      /// The size of the element in bytes.
      const size_t Size;

      const VertexElementUsage::Enum Usage;
      const unsigned int UsageIndex;

      /// The offset in bytes from the start of the vertex structure to this element.
      const size_t Offset;

      /// The data stream from which this element will be taken.
      const unsigned int Stream;
    };
  }
}

#endif // __THEIA_GFX_VERTEX_ELEMENT__
