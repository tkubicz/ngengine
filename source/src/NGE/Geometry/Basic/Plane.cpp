#include "NGE/Geometry/Basic/Plane.hpp"
#include "NGE/Core/Core.hpp"
using namespace NGE::Geometry::Basic;

bool Plane::Initialize()
{
    if (InitializeGeometry() && InitializeVBO() && InitializeVBA())
        return true;
	
	return false;
}

void Plane::DeleteBuffers()
{
    if (vertexBuffer != 0)
        glDeleteBuffers(1, &vertexBuffer);

    if (normalBuffer != 0)
        glDeleteBuffers(1, &normalBuffer);
    
    if (texCoordBuffer != 0)
        glDeleteBuffers(1, &vertexBuffer);

    if (tangentBuffer != 0)
        glDeleteBuffers(1, &normalBuffer);
    
    if (bitangentBuffer != 0)
        glDeleteBuffers(1, &vertexBuffer);

    if (indexBuffer != 0)
        glDeleteBuffers(1, &normalBuffer);
}

void Plane::DeleteArrays()
{
    if (vertexArray != 0)
        glDeleteVertexArrays(1, &vertexArray);
}

bool Plane::InitializeGeometry()
{
    unsigned vertexCount = 4;
    unsigned indexCount = 6;
    float planeSize = 2.0f;

    float xy_vertices[] = {-1.0f, -1.0f, 0.0f,
                           1.0f, -1.0f, 0.0f,
                           -1.0f, 1.0f, 0.0f,
                           1.0f, 1.0f, 0.0f};

    float xy_normals[] = {0.0f, 0.0f, 1.0f,
                          0.0f, 0.0f, 1.0f,
                          0.0f, 0.0f, 1.0f,
                          0.0f, 0.0f, 1.0f};

    float xy_tangents[] = {1.0f, 0.0f, 0.0f,
                           1.0f, 0.0f, 0.0f,
                           1.0f, 0.0f, 0.0f,
                           1.0f, 0.0f, 0.0f};

    float xy_texCoords[] = {0.0f, 0.0f,
                            1.0f, 0.0f,
                            0.0f, 1.0f,
                            1.0f, 1.0f};

    int xy_indices[] = {0, 1, 2, 1, 3, 2};

    vertices.reserve(vertexCount);
    texCoords.reserve(vertexCount);
    normals.reserve(vertexCount);
    tangents.reserve(vertexCount);
    bitangents.reserve(vertexCount);
    indices.reserve(indexCount);

    for (unsigned i = 0; i < vertexCount; ++i)
    {
        xy_vertices[i * 3 + 0] *= planeSize;
        xy_vertices[i * 3 + 1] *= planeSize;

        vertices.push_back(Math::vec3f(xy_vertices[i * 3 + 0], xy_vertices[i * 3 + 1], xy_vertices[i * 3 + 2]));
        normals.push_back(Math::vec3f(xy_normals[i * 3 + 0], xy_normals[i * 3 + 1], xy_normals[i * 3 + 2]));
        texCoords.push_back(Math::vec2f(xy_texCoords[i * 2 + 0], xy_texCoords[i * 2 + 1]));
        tangents.push_back(Math::vec3f(xy_tangents[i * 3 + 0], xy_tangents[i * 3 + 1], xy_tangents[i * 3 + 2]));

        // Bitangents
        bitangents.push_back(normals[i].CrossProduct(tangents[i]));
    }

    for (unsigned i = 0; i < indexCount; ++i)
        indices.push_back(xy_indices[i]);

    return true;
}

bool Plane::InitializeVBO()
{
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 3 * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 3 * normals.size(), &normals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 2 * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);

    glGenBuffers(1, &tangentBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 3 * tangents.size(), &tangents[0], GL_STATIC_DRAW);

    glGenBuffers(1, &bitangentBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 3 * bitangents.size(), &bitangents[0], GL_STATIC_DRAW);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

    return true;
}

bool Plane::InitializeVBA()
{
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, bitangentBuffer);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glBindVertexArray(0);

	return true;
}

void Plane::Update(float dt) { }

void Plane::Render() { }