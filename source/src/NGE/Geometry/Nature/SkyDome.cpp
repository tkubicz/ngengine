#include "NGE/Geometry/Nature/SkyDome.hpp"
#include "NGE/Math/MathUtils.hpp"
#include "NGE/Tools/Random.hpp"
#include "NGE/Rendering/Renderer.hpp"
#include "NGE/Core/Core.hpp"
using namespace NGE::Geometry::Nature;

SkyDome::SkyDome()
{
    skyRadius = 10.0f;
    turbidity = 2.0f;
    coloringMode = COLOR_MODEL;
}

int SkyDome::Initialize(float radius, int numSlices, int numSides, bool exponential, float dampening)
{
    this->numSides = numSides;
    this->numSlices = numSlices;
    this->numIndices = numSlices * (numSides + 1) * 2;
    skyRadius = radius;

    int size = (numSlices + 1) * (numSides + 1);
    vertices.reserve(size);
    colors.reserve(size);
    texCoords.reserve(size);
    skyMapTexCoords.reserve(size);
    indices.reserve(numIndices);

    float polyAng = 2.0f * (float) Math::MathUtils::PI / numSides;
    float ang;

    // Używane do teselacji
    exponential = false;

    float vX, vY, vZ;
    int i, j;

    for (j = 0; j <= numSlices; ++j)
    {
        ang = (float) j * (((float) Math::MathUtils::PI / 2.0f) / (float) numSlices);
        for (i = 0; i <= numSides; ++i)
        {
            vX = cos(i * polyAng) * cos(ang);
            vY = sin(ang) * dampening;
            vZ = sin(i * polyAng) * cos(ang);

            vertices.push_back(Math::vec3f(vX, vY, vZ) * skyRadius);
            texCoords.push_back(Math::vec2f(float(i) / float(numSides), float(j) / float(numSlices)));
            skyMapTexCoords.push_back(Math::vec2f(0, float(j) / float(numSlices)));

            colors.push_back(Math::vec4f(0.3f, 0.5f, 0.8f, 1.0f));
        }
    }

    for (j = 1; j <= numSlices; ++j)
    {
        for (i = 0; i <= numSides; ++i)
        {
            indices.push_back(j * (numSides + 1) + i);
            indices.push_back((j - 1) * (numSides + 1) + i);
        }
    }

    numStars = 1000;
    stars = new Math::vec3f[numStars];
    starColors = new Math::vec4f[numStars];

    // Star theta, star phi
    float st, sp, c;
    Tools::Random random;

    for (i = 0; i < numStars; ++i)
    {
        st = (random.RandomInt(90) / 180.0f * (float) Math::MathUtils::PI);
        sp = (random.RandomInt(360) / 180.0f * (float) Math::MathUtils::PI);

        stars[i].x = cos(sp) * cos(st) * skyRadius;
        stars[i].y = sin(st) * skyRadius * dampening;
        stars[i].z = sin(sp) * cos(st) * skyRadius;

        c = random.RandomInt(256) / 255.0f;

        starColors[i] = Math::vec4f(c, c, c, c);
    }

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * vertices.size() * 3, &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * texCoords.size() * 2, &texCoords[0], GL_STATIC_DRAW);

    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * colors.size() * 4, &colors[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer((GLint) 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glVertexAttribPointer((GLint) 1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer((GLint) 2, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glBindVertexArray(0);

    return 0;
}

void SkyDome::SetCoordinates(float latitude, float longitude, float timeOfDay, float julianDay, float turbidity)
{
    this->latitude = Math::MathUtils::DegToRad(latitude);
    this->longitude = Math::MathUtils::DegToRad(longitude);
    this->timeOfDay = timeOfDay * 3600.0f;
    this->julianDay = julianDay;
    this->turbidity = turbidity;
}

void SkyDome::Destroy() {
    // TODO: Wyczyszczenie zmiennych
}

void SkyDome::Update(float dt)
{
    timeOfDay += dt * 3000;

    // Następny dzień?
    if (timeOfDay > 3600 * 24)
    {
        julianDay += 1.0f;
        timeOfDay -= 3600 * 24;
    }

    float hour = timeOfDay / 3600.0f;
    float solarTime = hour + 0.170f * sin(4.0f * (float) Math::MathUtils::PI * (julianDay - 80.0f) / 373.0f) - 0.129f *
            std::sin(2.0f * (float) Math::MathUtils::PI * (julianDay - 8.0f) / 355.0f) + 12.0f * (latitude - longitude) / (float) Math::MathUtils::PI;
    float declination = 0.4093f * std::sin(2.0f * (float) Math::MathUtils::PI * (julianDay - 81.0f) / 368.0f);

    sunTheta = (float) Math::MathUtils::PI / 2.0f - std::asin(std::sin(latitude) * std::sin(declination) - std::cos(latitude) * std::cos(declination) * std::cos((float) Math::MathUtils::PI * solarTime / 12.0f));
    sunPhi = std::atan(-std::cos(declination) * std::sin((float) Math::MathUtils::PI * solarTime / 12.0f) /
                       ((std::cos(latitude) * std::cos(declination) - std::sin(latitude) * std::sin(declination) * std::sin((float) Math::MathUtils::PI * solarTime / 12.0f))));

    if (sunPhi < 0)
        sunPhi = 2.0f * (float) Math::MathUtils::PI + sunPhi;

    moonPhi = (float) Math::MathUtils::PI / 2.0f;
    moonTheta = (float) Math::MathUtils::PI / 3.0f;

    int ind, num;

    float halfPi = (float) Math::MathUtils::PI / 2.0f;
    sliceAng = halfPi / numSlices;
    sideAng = (2.0f * (float) Math::MathUtils::PI) / (float) numSides;

    float theta, phi;
    int index = 0;

    skyColor.SetInfo(sunTheta, turbidity);

    for (ind = 0; ind <= numSlices; ++ind)
    {
        for (num = 0; num <= numSides; ++num)
        {
            theta = halfPi - sliceAng * ind;
            phi = num * sideAng;

            if (coloringMode == COLOR_MODEL)
            {
                skyColor.GetVertexColor(theta, phi, sunPhi, &colors[index]);
            }

            index++;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof (GLfloat) * 4 * colors.size(), &colors[0]);
}

void SkyDome::Render()
{
    Rendering::Renderer::GetInstance().GetMatrixStack().PushMatrix();

    // Odkomentować, jeżeli niebo ma podążać za kamerą, tak by nie można było dosięgnąć krawędzi
    /*if (camera != NULL)
        Rendering::Renderer::GetInstance().GetMatrixStack().Translate(camera->GetViewerPosition());
     */

    glDisable(GL_DEPTH_TEST);

    if (coloringMode != COLOR_TEXTURE)
        RenderElements(true);

    glEnable(GL_DEPTH_TEST);
    RenderDome();
    glDisable(GL_DEPTH_TEST);

    if (coloringMode != COLOR_TEXTURE)
        RenderElements(false);

    glEnable(GL_DEPTH_TEST);

    Rendering::Renderer::GetInstance().GetMatrixStack().PopMatrix();
}

NGE::Math::vec4f SkyDome::GetHorizonColor(const NGE::Math::vec3f& cameraView)
{
    float ang = atan2(cameraView.z, cameraView.x);
    if (ang < 0)
        ang = 2.0f * (float) Math::MathUtils::PI + ang;
    int ri = int((ang / (2 * Math::MathUtils::PI)) * numSides);

    return Math::vec4f(colors[ri]);
}

void SkyDome::RenderElements(bool initial) {
 }

void SkyDome::RenderDome()
{
    /*int i;
    bool useTexture = ((coloringMode == COLOR_TEXTURE) || (coloringMode == COLOR_SKYMAP));
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    if (useTexture)
    {
        if (coloringMode == COLOR_TEXTURE)
            tex->Activate();
        else if (coloringMode == COLOR_SKYMAP)
            skyMap->Activate();
    }
     */

    if (shader == NULL || tex == NULL)
        return;

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    shader->BindShader();
    tex->Activate();

    shader->SendUniform4x4("modelview_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX));
    shader->SendUniform4x4("projection_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));
    shader->SendUniform("texture0", 0);

    glBindVertexArray(vertexArray);
    glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    shader->UnbindShader();
    tex->Deactivate();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void SkyDome::SetShader(NGE::Media::Shaders::GLSLProgram* shader)
{
    this->shader = shader;
}

void SkyDome::SetTexture(NGE::Media::Images::Texture* texture)
{
    this->tex = texture;
}

void SkyDome::SetCamera(NGE::Rendering::Camera::Camera* camera)
{
    this->camera = camera;
}

void SkyDome::DeleteBuffers()
{
    if (vertexBuffer != 0)
        glDeleteBuffers(1, &vertexBuffer);

    if (texCoordBuffer != 0)
        glDeleteBuffers(1, &texCoordBuffer);

    if (indexBuffer != 0)
        glDeleteBuffers(1, &normalBuffer);

    if (colorBuffer != 0)
        glDeleteBuffers(1, &colorBuffer);
}

void SkyDome::DeleteArrays()
{
    if (vertexArray != 0)
        glDeleteVertexArrays(1, &vertexArray);
}