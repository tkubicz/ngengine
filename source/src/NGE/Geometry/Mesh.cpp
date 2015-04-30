#include "NGE/Geometry/Mesh.hpp"
using namespace NGE::Geometry;

Mesh::Mesh()
{
    vertexArray = vertexBuffer = texCoordBuffer = colorBuffer = normalBuffer = indexBuffer = tangentBuffer = bitangentBuffer = 0;
}

Mesh::~Mesh() { }

GLuint& Mesh::GetVertexArray()
{
    return vertexArray;
}

GLuint& Mesh::GetVertexBuffer()
{
    return vertexBuffer;
}

GLuint& Mesh::GetTexCoordBuffer()
{
    return texCoordBuffer;
}

GLuint& Mesh::GetNormalBuffer()
{
    return normalBuffer;
}

GLuint& Mesh::GetColorBuffer()
{
    return colorBuffer;
}

GLuint& Mesh::GetIndexBuffer()
{
    return indexBuffer;
}

GLuint& Mesh::GetTangentBuffer()
{
    return tangentBuffer;
}

GLuint& Mesh::GetBitangentBuffer()
{
    return bitangentBuffer;
}

std::vector<NGE::Math::vec3f>& Mesh::GetVertices()
{
    return vertices;
}

std::vector<NGE::Math::vec2f>& Mesh::GetTexCoords()
{
    return texCoords;
}

std::vector<NGE::Math::vec3f>& Mesh::GetNormals()
{
    return normals;
}

std::vector<NGE::Math::vec4f>& Mesh::GetColors()
{
    return colors;
}

std::vector<GLuint>& Mesh::GetIndices()
{
    return indices;
}

std::vector<NGE::Math::vec3f>& Mesh::GetTangents()
{
    return tangents;
}

std::vector<NGE::Math::vec3f>& Mesh::GetBitangents()
{
    return bitangents;
}

void Mesh::DeleteBuffers() { }

void Mesh::DeleteArrays() { }

std::vector<Face> Mesh::GetFaces() const
{
    return faces;
}

unsigned int Mesh::GetNumFaces() const
{
    return numFaces;
}

void Mesh::SetNumFaces(unsigned int numFaces)
{
    this->numFaces = numFaces;
}


