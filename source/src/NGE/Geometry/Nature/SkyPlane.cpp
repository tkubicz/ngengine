#include "NGE/Geometry/Nature/SkyPlane.hpp"
#include "NGE/Math/MathUtils.hpp"
#include "NGE/Rendering/Renderer.hpp"
#include "NGE/Core/Core.hpp"
using namespace NGE::Geometry::Nature;

void SkyPlane::GenerateSkyPlane(int divisions, float planetRadius, float athmosphereRadius, float hTile, float vTile, float alphaPercent, bool exponentialFadeout) {
	vertices.clear();
	indices.clear();

	alphaPercent = Math::MathUtils::Clamp(alphaPercent, 0.0f, 1.0f);

	int divs = divisions;
	if (divisions < 1) divs = 1;

	vertices.reserve((divs + 1) * (divs + 1));
	alphas.reserve((divs + 1) * (divs + 1));
	texCoords.reserve((divs + 1) * (divs + 1));
	indices.reserve(divs * divs * 2 * 3);

	float planeSize = sqrt(((2.0f * planetRadius) * (2.0f * planetRadius)) * 0.5f);
	float delta = planeSize / float(divs);
	float texDelta = 1.0f / float(divs);

	float xDist = 0.0f;
	float zDist = 0.0f;
	float xHeight = 0.0f;
	float zHeight = 0.0f;
	float height = 0.0f;

	float alpharadiusSquared = (planeSize * alphaPercent * 0.5f) * (planeSize * alphaPercent * 0.5f);
	float planetradiusSquared = planetRadius * planetRadius;

	Math::vec3f vertex;
	Math::vec2f tex;
	float alphaTemp;

	for (int i = 0; i <= divs; ++i) {
		for (int j = 0; j <= divs; ++j) {
			xDist = (-0.5f * planeSize) + (float(j) * delta);
			zDist = (-0.5f * planeSize) + (float(i) * delta);

			xHeight = xDist * xDist;
			zHeight = zDist * zDist;
			height = (xHeight + zHeight) / planetradiusSquared;

			vertex.x = xDist;
			vertex.y = (1.0f - height) * athmosphereRadius;
			vertex.z = zDist;
			alphaTemp = (xHeight + zHeight) / alpharadiusSquared;

			if (exponentialFadeout)
				alphaTemp = alphaTemp * alphaTemp * alphaTemp;

			alphaTemp = 1.0f - alphaTemp;

			if (alphaTemp < 0.0f) alphaTemp = 0.0f;

			tex.x = hTile * (float(j) * texDelta);
			tex.y = vTile * (float(i) * texDelta);

			//vertices[i * (divs + 1) + j] = vertex;
			//alphas[i * (divs + 1) + j] = alphaTemp;
			//texCoords[i * (divs + 1) + j] = tex;
			vertices.push_back(vertex);
			alphas.push_back(alphaTemp);
			texCoords.push_back(tex);
		}
	}

	for (int i = 0; i < divs; ++i) {
		for (int j = 0; j < divs; ++j) {
			int startvert = i * (divs + 1) + j;

			// Pierwszy trójkąt
			indices.push_back(startvert);
			indices.push_back(startvert + 1);
			indices.push_back(startvert + divs + 1);

			// Drugi trójkąt
			indices.push_back(startvert + 1);
			indices.push_back(startvert + divs + 2);
			indices.push_back(startvert + divs + 1);
		}
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


	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer((GLint) 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glVertexAttribPointer((GLint) 1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glBindVertexArray(0);
}

void SkyPlane::Render(int alpha) {
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (shader == NULL || texture == NULL)
		return;

	shader->bindShader();
	texture->Activate();

	shader->sendUniform4x4("modelview_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX));
	shader->sendUniform4x4("projection_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));
	shader->sendUniform("texture0", 0);

	glBindVertexArray(vertexArray);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	shader->unbindShader();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void SkyPlane::SetShader(NGE::Media::Shaders::GLSLProgram* shader) {
	this->shader = shader;
}

void SkyPlane::SetTexture(NGE::Media::Images::Texture* texture) {
	this->texture = texture;
}

void SkyPlane::DeleteBuffers() {
	if (vertexBuffer != 0)
		glDeleteBuffers(1, &vertexBuffer);

	if (texCoordBuffer != 0)
		glDeleteBuffers(1, &texCoordBuffer);

	if (indexBuffer != 0)
		glDeleteBuffers(1, &normalBuffer);
}

void SkyPlane::DeleteArrays() {
	if (vertexArray != 0)
		glDeleteVertexArrays(1, &vertexArray);
}

