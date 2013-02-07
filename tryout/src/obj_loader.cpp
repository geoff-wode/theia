
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <ctype.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <gl_loader.h>

//----------------------------------------------

struct Vertex
{
  glm::vec3 p;
  glm::vec3 n;
  glm::vec2 t;
};

struct Face
{
  unsigned int  v[3];
  unsigned int  t[3];
  unsigned int  n[3];
};

//----------------------------------------------

static void Read(const std::string& line, size_t pos, float* pv)
{
  // Read float values from a space-delimited data stream. If the read fails
  // the function returns immediately.
  std::istringstream s(line[pos]);
  while (s >> *pv)
  {
    ++pv;
  }
}

//----------------------------------------------

static void ReadFace(const std::string& line, size_t pos, Face& face)
{
  std::istringstream s(line[pos]);

  for (size_t i = 0; i < 3; ++i)
  {
    char c;
    face.v[i] = 0;
    face.n[i] = 0;
    face.t[i] = 0;

    s >> face.v[i];
    if ('/' == s.peek())
    {
      s.get(c);
      if ('/' == s.peek())
      {
        s.get(c);
        s >> face.n[i];
      }
      else
      {
        s >> face.t[i];
        if ('/' == s.peek())
        {
          s.get(c);
          s >> face.n[i];
        }
      }
    }
  }
}

//----------------------------------------------

void ReadOBJ(const char* file, std::vector<Vertex>& vertices)
{
  std::vector<glm::vec4> vertexList;
  std::vector<glm::vec3> texcoordsList;
  std::vector<glm::vec3> normalsList;
  std::vector<Face>      faces;

  std::istringstream stream(file);
  std::string line;
  while (std::getline(stream, line))
  {
    switch (line[0])
    {
    case 'v':
      switch (line[1])
      {
      case 'n': // vertex normal
        {
          glm::vec3 n(0);
          Read(line, 2, glm::value_ptr(n));
          glm::normalize(n);
          normalsList.push_back(n);
        }
        break;
      case 't': // texture coordinate
        {
          glm::vec3 t(0);
          Read(line, 2, glm::value_ptr(t));
          texcoordsList.push_back(t);
        }
        break;
      case 'p': // parameter space vertex
        break;
      default:  // plain vertex coordinate
        {
          glm::vec4 v(0);
          Read(line, 1, glm::value_ptr(v));
          vertexList.push_back(v);
        }
        break;
      }
      break;
    case 'f':   // face definition
      {
        Face f;
        ReadFace(line, 1, f);
        faces.push_back(f);
      }
      break;
    }
  }

  for (size_t f = 0; f < faces.size(); ++f)
  {
    Vertex v;
    for (int i = 0; i < 3; ++i)
    {
      v.p = glm::vec3(vertexList[faces[f].v[i]]);
      if (!normalsList.empty())
      {
        v.n = normalsList[faces[f].n[i]];
      }
      if (!texcoordsList.empty())
      {
        v.t = glm::vec2(texcoordsList[faces[f].t[i]]);
      }
    }
  }
}
