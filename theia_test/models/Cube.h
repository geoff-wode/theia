namespace Cube
{
  struct Vertex
  {
    glm::vec3 position;
  };
  static const size_t VertexDeclData[] =
  {
    sizeof(Vertex), // vertex stride
    1,  // # elements

   GL_FLOAT, 3, offsetof(Vertex, position)
  };

  static const VertexDeclaration* VertexDecl = (const VertexDeclaration*)VertexDeclData;

  static const Vertex Vertices[] =
  {
    glm::vec3(-1.000000, -1.000000, 1.000000  ),
    glm::vec3(-1.000000, 1.000000, 1.000000   ),
    glm::vec3(-1.000000, 1.000000, -1.000000  ),
    glm::vec3(1.000000, 1.000000, 1.000000    ),
    glm::vec3(1.000000, 1.000000, -1.000000   ),
    glm::vec3(1.000000, -1.000000, 1.000000   ),
    glm::vec3(1.000000, -1.000000, -1.000000  ),
    glm::vec3(-1.000000, -1.000000, -1.000000 )
  };
  static const size_t NumVertices = sizeof(Vertices)/sizeof(Vertices[0]);

  static const unsigned short Indices[] =
  {
    0, 1, 2,
    1, 3, 4,
    3, 5, 6,
    0, 7, 5,
    7, 2, 4,
    5, 3, 1,
    7, 0, 2,
    2, 1, 4,
    4, 3, 6,
    5, 7, 6,
    6, 7, 4,
    0, 5, 1
  };
  static const size_t NumIndices = sizeof(Indices)/sizeof(Indices[0]);

} // Cube
