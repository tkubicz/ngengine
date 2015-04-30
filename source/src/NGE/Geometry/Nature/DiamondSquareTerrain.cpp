#include "NGE/Geometry/Nature/DiamonSquareTerrain.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "NGE/Rendering//Renderer.hpp"
#include "NGE/Core/Core.hpp"
using namespace NGE::Geometry::Nature;

DiamonSquareTerrain::DiamonSquareTerrain(unsigned int detail)
{
    size = pow(2, detail) + 1;
    max = size - 1;
    //heights.resize(size * size);
}

void DiamonSquareTerrain::Generate(const float roughness)
{
    random = new Tools::Random();
    vertices.clear();
    indices.clear();
    colors.clear();
    heights.clear();
    heights.resize(size * size);
    this->roughness = roughness;

    /*SetHeight(0, 0, max);
    SetHeight(max, 0, max / 2);
    SetHeight(max, max, 0);
    SetHeight(0, max, max / 2);*/

    SetHeight(0, 0, Random());
    SetHeight(max, 0, Random());
    SetHeight(max, max, Random());
    SetHeight(0, max, Random());

    Divide(max);
}

float DiamonSquareTerrain::GetHeight(const int x, const int y) const
{
    if (x < 0 || x > max || y < 0 || y > max)
        return -1;
    return heights[x + size * y];
}

void DiamonSquareTerrain::SetHeight(const int x, const int y, const float value)
{
    heights[x + size * y] = value;
}

float DiamonSquareTerrain::Average(const std::vector<float>& input)
{
    float sum = 0.0f;
    unsigned int size = input.size();
    for (float i : input)
    {
        if (i != -1)
            sum += i;
        else
            size--;
    }

    return sum / size;
}

void DiamonSquareTerrain::Square(const int x, const int y, const int size, const double offset)
{
    float avg = Average({
                        GetHeight(x - size, y - size), // Upper left.
                        GetHeight(x + size, y - size), // Upper right.
                        GetHeight(x + size, y + size), // Lower right.
                        GetHeight(x - size, y + size) // Lower left.
    });
    SetHeight(x, y, avg + offset);
}

void DiamonSquareTerrain::Diamond(const int x, const int y, const int size, const double offset)
{
    float avg = Average({
                        GetHeight(x, y - size), // Top.
                        GetHeight(x + size, y), // Right.
                        GetHeight(x, y + size), // Bottom.
                        GetHeight(x - size, y) // Left.
    });
    SetHeight(x, y, avg + offset);
}

void DiamonSquareTerrain::Divide(const int size)
{
    int x, y, half = size / 2;
    double scale = (double) roughness * (double) size;

    if (half < 1)
        return;

    for (y = half; y < max; y += size)
    {
        for (x = half; x < max; x += size)
        {
            Square(x, y, half, Random() * scale);
        }
    }

    for (y = 0; y <= max; y += half)
    {
        for (x = (y + half) % size; x <= max; x += size)
        {
            Diamond(x, y, half, Random() * scale);
        }
    }

    Divide(size / 2);
}

double DiamonSquareTerrain::Random()
{
    return 2.0 * (double) random->RandomFloat() - 1.0;
}

void DiamonSquareTerrain::GenerateVertices()
{
    float blockScale = 1.0f;
    float heightScale = 1.0f;
    float terrainWidth = size, terrainHeight = size;

    float terrainBlockWidth = (terrainWidth - 1) * blockScale;
    float terrainBlockHeight = (terrainHeight - 1) * blockScale;

    float halfTerrainBlockWidth = terrainBlockWidth * 0.5f;
    float halfTerrainBlockHeight = terrainBlockHeight * 0.5f;

    unsigned int numVertices = terrainHeight * terrainWidth;
    vertices.reserve(numVertices);
    colors.resize(numVertices);

    for (int height = 0; height < terrainHeight; ++height)
    {
        for (int width = 0; width < terrainWidth; ++width)
        {
            int index = (height * terrainWidth) + width;
            float S = float(width) / float(terrainWidth - 1);
            float T = float(height) / float(terrainHeight - 1);

            float X = (S * terrainBlockWidth) - halfTerrainBlockWidth;
            float Y = heights[index] * heightScale;
            float Z = (T * terrainBlockHeight) - halfTerrainBlockHeight;

            vertices.push_back(Math::vec3f(X, Y, Z));

            //float tex0Contribution = 1.0f - GetPercentage(heights[index], 0.0f, 0.75f);
            //float tex2Contribution = 1.0f - GetPercentage(heights[index], 0.75f, 1.0f);
            //colors[index] = Math::vec4f(tex0Contribution, tex0Contribution, tex0Contribution, tex2Contribution);
        }
    }

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (float) * 3 * vertices.size(), &vertices[0], GL_STATIC_DRAW);
}

void DiamonSquareTerrain::GenerateIndices()
{
    float terrainHeight = size, terrainWidth = size;

    if (terrainHeight < 2 || terrainWidth < 2)
    {
        //Tools::Logger::WriteErrorLog("Terrain --> Terrain hasn't been loaded, or is of incorrect size");
        return;
    }

    const unsigned int numTriangles = (terrainWidth - 1) * (terrainHeight - 1) * 2;
    indices.resize(numTriangles * 3);

    unsigned int vertexIndex, index = 0;
    for (unsigned int height = 0; height < unsigned(terrainHeight - 1); ++height)
    {
        for (unsigned int width = 0; width < unsigned(terrainWidth - 1); ++width)
        {
            vertexIndex = (height * terrainWidth) + width;
            // Górny trójkąt
            indices[index++] = vertexIndex; // V0
            indices[index++] = vertexIndex + terrainWidth + 1; // V3
            indices[index++] = vertexIndex + 1; // V1
            // Dolny trójkąt
            indices[index++] = vertexIndex; // V0
            indices[index++] = vertexIndex + terrainWidth; // V2
            indices[index++] = vertexIndex + terrainWidth + 1; // V3
        }
    }

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

    indicesSize = indices.size();
}

void DiamonSquareTerrain::GenerateVBA()
{
    glGenVertexArrays(1, &vba);
    glBindVertexArray(vba);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer((GLint) 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glBindVertexArray(0);
}

void DiamonSquareTerrain::DestroyGeometry()
{
    glBindVertexArray(vba);

    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);

    glBindVertexArray(0);

    glDeleteVertexArrays(1, &vba);

    vertexBuffer = indexBuffer = vba = 0;
}

void DiamonSquareTerrain::Render()
{
#ifndef ANDROID
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
    shader->BindShader();

    shader->SendUniform4x4("modelviewMatrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX));
    shader->SendUniform4x4("projectionMatrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));

    glBindVertexArray(vba);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    shader->UnbindShader();
}
