

#if ! defined(__VERTEX__)
#define __VERTEX__

#include <gl_loader.h>

// Vertex data is made up of structures containing one or more elements.
struct VertexElement
{
  const GLenum  type;         // type of the components of the element (float, etc.)
  const size_t  components;   // number of components (e.g. == 3 for vec3)
  const size_t  offset;       // offset in bytes from start of vertex to this element
};

// Declares the in-memory layout of a vertex buffer.
struct VertexDeclaration
{
  const size_t  stride;                 // distance from start of one vertex to the next
  const size_t  numElements;            // the number of elements comprising one vertex
  const VertexElement* const elements;  // array of the elements
};

#endif // __VERTEX__
