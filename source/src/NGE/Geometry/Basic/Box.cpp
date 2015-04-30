#include "NGE/Geometry/Basic/Box.hpp"
#include "NGE/Math/Vector3.hpp"
#include "NGE/Rendering/Renderer.hpp"
#include "NGE/Core/Core.hpp"
using namespace NGE::Geometry::Basic;

bool Box::Initialize()
{
    return Initialize(1.0f);
}

bool Box::Initialize(float size)
{
    this->boxSize = size / 2.0f;

    InitializeGeometry();
    InitializeVBO();
    InitializeVBA();

    return true;
}

void Box::DeleteBuffers()
{
    if (vertexBuffer != 0)
        glDeleteBuffers(1, &vertexBuffer);

    if (normalBuffer != 0)
        glDeleteBuffers(1, &normalBuffer);

    if (texCoordBuffer != 0)
        glDeleteBuffers(1, &texCoordBuffer);

    if (indexBuffer != 0)
        glDeleteBuffers(1, &indexBuffer);
}

void Box::DeleteArrays()
{
    if (vertexArray != 0)
        glDeleteVertexArrays(1, &vertexArray);
}

bool Box::InitializeGeometry()
{
    int vertexCount = 24;
    int indexCount = 36;

    float vertices[] = {-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f, -1.0f, -1.0f,
                        -1.0f, +1.0f, -1.0f, -1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, -1.0f,
                        -1.0f, -1.0f, -1.0f, -1.0f, +1.0f, -1.0f, +1.0f, +1.0f, -1.0f, +1.0f, -1.0f, -1.0f,
                        -1.0f, -1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f,
                        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, +1.0f, -1.0f, +1.0f, +1.0f, -1.0f, +1.0f, -1.0f,
                        +1.0f, -1.0f, -1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, -1.0f};

    float normals[] = {0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
                       0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f,
                       0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
                       0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f,
                       -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                       +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f};

    float texCoords[] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                         0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                         1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                         0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                         1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};

    unsigned int indices[] = {0, 2, 1, 0, 3, 2, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 15, 14, 12, 14, 13, 16, 17, 18, 16, 18, 19, 20, 23, 22, 20, 22, 21};


    this->vertices.reserve(vertexCount);
    this->texCoords.reserve(vertexCount);
    this->normals.reserve(vertexCount);
    this->indices.reserve(indexCount);

    for (int i = 0; i < vertexCount; ++i)
    {
        vertices[i * 3 + 0] *= boxSize;
        vertices[i * 3 + 1] *= boxSize;
        vertices[i * 3 + 2] *= boxSize;

        this->vertices.push_back(Math::vec3f(vertices[i * 3 + 0], vertices[i * 3 + 1], vertices[i * 3 + 2]));
        this->normals.push_back(Math::vec3f(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]));
        this->texCoords.push_back(Math::vec2f(texCoords[i * 2 + 0], texCoords[i * 2 + 1]));
    }

    for (int i = 0; i < indexCount; ++i)
        this->indices.push_back(indices[i]);

    return true;
}

bool Box::InitializeVBO()
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

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

    return true;
}

bool Box::InitializeVBA()
{
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glBindVertexArray(0);

    return true;
}

void Box::SetShader(NGE::Media::Shaders::GLSLProgram* shader)
{
    this->shader = shader;
}

void Box::SetTexture(NGE::Media::Images::Texture* texture)
{
    this->texture = texture;
}

void Box::Update(float deltaTime) { }

void Box::Render()
{
    if (shader == NULL)
        return;

    shader->BindShader();

    //texture->Activate();

    shader->SendUniform4x4("modelview_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX));
    shader->SendUniform4x4("projection_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));
    shader->SendUniform("in_color", Math::vec4f(0.55f, 0.26f, 0.27f));
    //shader->SendUniform("texture0", 0);

    glBindVertexArray(vertexArray);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    shader->UnbindShader();
}
