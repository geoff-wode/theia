#include <SDL.h>
#include <stdint.h>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

struct Face
{
  unsigned int v;
  unsigned int t;
  unsigned int n;
};

typedef std::vector<glm::vec4> VertexList;
typedef std::vector<glm::vec3> NormalsList;
typedef std::vector<glm::vec3> TextureCoordList;
typedef std::vector<Face> FacesList;

static bool haveNormals = false;
static bool haveTextureCoords = false;
static int vertexComponents = 0;
static int textureCoordComponents = 0;

static FILE* outfile = NULL;

static void AddVertex(const std::string& line, VertexList& list)
{
  float x, y, z, w;
  w = 1.0f;
  vertexComponents = sscanf(line.c_str() + 1, "%f %f %f %f", &x, &y, &z, &w);
  list.push_back(glm::vec4(x, y, z, w));
}

static void AddNormal(const std::string& line, NormalsList& list)
{
  float x, y, z;
  sscanf(line.c_str() + 2, "%f %f %f", &x, &y, &z);
  list.push_back(glm::vec3(x, y, z));
}

static void AddTextureCoord(const std::string& line, TextureCoordList& list)
{
  float u, v, w;
  w = 0.0f;
  textureCoordComponents = sscanf(line.c_str() + 2, "%f %f %f", &u, &v, &w);
  list.push_back(glm::vec3(u, v, w));
}

static void AddFace(const std::string& line, FacesList& list)
{
  Face face;
  const char* s = line.c_str()+1;
  sscanf(s, "%d", &face.v);
  s = strchr(s, '/');
  if (NULL != s)
  {
    ++s;
    if (*s != '/')
    {
      sscanf(s, "%d", &face.t);
      haveTextureCoords = true;
    }
  }
  s = strchr(s, '/');
  if (NULL != s)
  {
    ++s;
    sscanf(s, "%d", &face.n);
    haveNormals = true;
  }

  list.push_back(face);
}

static void DumpObjectToLog(
  const VertexList& vertices,
  const NormalsList& normals,
  const  TextureCoordList& textureCoords,
  const FacesList& faces)
{
  const int numElements = 1 + (int)haveNormals + (int)haveTextureCoords;
  const int vertexStride = sizeof(float) * (vertexComponents + textureCoordComponents + (3 * (int)haveNormals));

  // Define the vertex structure...
  fprintf(outfile, "struct Vertex\n{\n");
  fprintf(outfile, "  glm::vec%d position;\n", vertexComponents);
  if (haveNormals) { fprintf(outfile,"  glm::vec3 normal;\n"); }
  if (haveTextureCoords) { fprintf(outfile,"  glm::vec%d texCoord;\n", textureCoordComponents); }
  fprintf(outfile, "};\n\n");

  // Define the vertex declaration...
  fprintf(outfile, "static const size_t declData =\n{\n");
  fprintf(outfile, "  %d, // vertex stride\n  %d,  // # elements\n", vertexStride, numElements);
  fprintf(outfile, "  {\n    { GL_FLOAT, %d, offsetof(Vertex, position) }", vertexComponents);
  if (haveNormals) { fprintf(outfile, ",\n    { GL_FLOAT, 3, offsetof(Vertex, normal) }"); }
  if (haveTextureCoords) { fprintf(outfile, "\n,    { GL_FLOAT, %d, offsetof(Vertex, texCoord) }", textureCoordComponents); }
  fprintf(outfile, "\n  }\n");
  fprintf(outfile, "};\n");
  fprintf(outfile, "static const VertexDeclaration* VertexDecl = (const VertexDeclaration*)declData;\n\n");

  // Define the vertex data...
  fprintf(outfile, "static const Vertex Vertices[] =\n{\n");
  for (size_t f = 0; f < faces.size(); ++f)
  {
    const glm::vec4* p = &vertices[faces[f].v-1];
    if (vertexComponents == 3) { fprintf(outfile, "  { glm::vec3(%ff, %ff, %ff)", p->x, p->y, p->z); }
    else { fprintf(outfile, "  { glm::vec4(%ff, %ff, %ff, %ff)", p->x, p->y, p->z, p->w); }

    if (haveNormals)
    {
      const glm::vec3* n = &normals[faces[f].n-1];
      fprintf(outfile, ", glm::vec3(%ff, %ff, %ff)", n->x, n->y, n->z);
    }

    if (haveTextureCoords)
    {
      const glm::vec3* t = &textureCoords[faces[f].t]-1;
      if (textureCoordComponents == 2) { fprintf(outfile, ", glm::vec2(%ff, %ff)", t->x, t->y); }
      else { fprintf(outfile, ", glm::vec3(%ff, %ff, %ff)", t->x, t->y, t->z); }
    }

    fprintf(outfile, " }%c\n", (f == faces.size()-1) ? ' ' : ',');
  }
  fprintf(outfile, "};\n");
  fprintf(outfile, "static const size_t NumVertices = sizeof(Vertices)/sizeof(Vertices[0]);\n\n");

  // Index data...
  fprintf(outfile, "static uint16_t Indices[] =\n{\n");
  for (size_t i = 0; i < faces.size(); ++i)
  {
  }
  fprintf(outfile, "};\nstatic const size_t NumIndices = sizeof(Indices)/sizeof(Indices[0]);\n");
}

void ParseMesh(std::istream& stream)
{
  std::string objectName("object");
  VertexList vertices;
  NormalsList normals;
  TextureCoordList textureCoords;
  FacesList faces;
  haveNormals = false;
  haveTextureCoords = false;
  textureCoordComponents = 0;

  // Read lines of text until end...
//  std::istringstream stream(text);
  std::string line;
  while (std::getline(stream, line))
  {
    switch (line[0])
    {
    case 'o': objectName = line.c_str() + 2; break;
    case 'v':
      switch (line[1])
      {
      case ' ': AddVertex(line, vertices); break;
      case 't': AddTextureCoord(line, textureCoords); break;
      case 'n': AddNormal(line, normals); break;
      }
      break;
    case 'f':
      AddFace(line, faces);
      break;
    default:
      break;
    }
  }

  std::string filename(objectName + ".h");
  outfile = fopen(filename.c_str(), "wb");
  fprintf(outfile, "namespace %s\n{\n", objectName.c_str());
  DumpObjectToLog(vertices, normals, textureCoords, faces);
  fprintf(outfile, "} // %s\n", objectName.c_str());
  fclose(outfile);
}

int main(int argc, char* argv[])
{
  SDL_Init(SDL_INIT_VIDEO);

  std::ifstream stream;
  stream.open(argv[1]);

  ParseMesh(stream);

  SDL_Quit();
  return 0;
}
