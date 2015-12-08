#include "NGE/Geometry/Basic/Sphere.hpp"
#include "NGE/Rendering/MatrixStack.hpp"
#include "NGE/Rendering/Renderer.hpp"
#include "NGE/Math/Quaternion.hpp"
#include "NGE/Math/MathUtils.hpp"
#include "NGE/Core/Core.hpp"

using namespace NGE::Geometry::Basic;

bool Sphere::Initialize() {
	return Initialize(24, 1.0f);
}

bool Sphere::Initialize(unsigned int numberSlices, float radius, NGE::Media::Shaders::GLSLProgram* program) {
	this->shader = program;
	this->sphereRadius = radius;
	this->numberSlices = numberSlices;

	if (!InitializeGeometry() || !InitializeVBO() || !InitializeVBA())
		return false;

	return true;
}

void Sphere::DeleteBuffers() {
	if (vertexBuffer != 0)
		glDeleteBuffers(1, &vertexBuffer);

	if (texCoordBuffer != 0)
		glDeleteBuffers(1, &texCoordBuffer);

	if (indexBuffer != 0)
		glDeleteBuffers(1, &indexBuffer);
}

void Sphere::DeleteArrays() {
	if (vertexArray != 0)
		glDeleteVertexArrays(1, &vertexArray);
}

bool Sphere::InitializeGeometry() {
	/*float x, y, z, s, t;
	x = y = z = s = t = 0.0f;

	vertices.clear();

	float PIOverStacks = (float) Math::MathUtils::PI / float(stacks);
	float PI2OverSlices = 2.0f * (float) Math::MathUtils::PI / float(slices);

	for (int stack = 0; stack < stacks; ++stack)
	{
		unsigned size = vertices.size();

		float Phi = float(stack) * PIOverStacks;
		float CosP = cos(Phi);
		float SinP = sin(Phi);

		for (int slice = 0; slice < slices; ++slice)
		{
			float Theta = float(slice) * PI2OverSlices;
			x = sphereRadius * cos(Theta) * SinP;
			y = sphereRadius * sin(Theta) * SinP;
			z = sphereRadius * CosP;
			s = 1.0f - (float) slice / (float) slices;
			t = (float) stack / (float) stacks;

			texCoords.push_back(Math::vec2f(s, t));
			vertices.push_back(Math::vec3f(x, y, z));
			Math::vec3f n1(x, y, z);
			n1.Normalize();
			normals.push_back(n1);

			float nextPhi = float(stack + 1) * PIOverStacks;
			float nextCosP = cos(nextPhi);
			float nextSinP = sin(nextPhi);
			x = sphereRadius * cos(Theta) * nextSinP;
			y = sphereRadius * sin(Theta) * nextSinP;
			z = sphereRadius * nextCosP;
			s = 1.0f - (float) slice / (float) slices;
			t = (float) (stack + 1.0f) / (float) stacks;

			texCoords.push_back(Math::vec2f(s, t));
			vertices.push_back(Math::vec3f(x, y, z));
			Math::vec3f n2(x, y, z);
			n2.Normalize();
			normals.push_back(n2);
		}

		vertices.push_back(vertices[size]);
		texCoords.push_back(texCoords[size]);
		normals.push_back(normals[size]);

		vertices.push_back(vertices[size + 1]);
		texCoords.push_back(texCoords[size + 1]);
		normals.push_back(normals[size + 1]);
	}*/

	unsigned int numberParallels = numberSlices / 2;
	unsigned int numberVertices = (numberParallels + 1) * (numberSlices + 1);
	unsigned int numberIndices = numberParallels * numberSlices * 6;

	float angleStep = (2.0f * (float) Math::MathUtils::PI) / ((float) numberSlices);

	// Used to calculate tangents vectors
	Math::vec3f helpVector(1, 0, 0);
	Math::quaternionf helpQuaternion;
	Math::mat4f helpMatrix;

	if (numberSlices < 3)
		return false;

	vertices.reserve(numberVertices);
	normals.reserve(numberVertices);
	tangents.reserve(numberVertices);
	texCoords.reserve(numberVertices);
	indices.reserve(numberIndices);

	for (unsigned int i = 0; i < numberParallels + 1; ++i) {
		for (unsigned int j = 0; j < numberSlices + 1; ++j) {
			vertices.push_back(
					Math::vec3f(sphereRadius * sin(angleStep * (float) i) * sin(angleStep * (float) j),
					sphereRadius * cos(angleStep * (float) i),
					sphereRadius * sin(angleStep * (float) i) * cos(angleStep * (float) j))
					);

			normals.push_back(
					Math::vec3f(vertices[vertices.size() - 1].x / sphereRadius,
					vertices[vertices.size() - 1].y / sphereRadius,
					vertices[vertices.size() - 1].z / sphereRadius)
					);

			texCoords.push_back(
					Math::vec2f((float) j / (float) numberSlices,
					1.0f - (float) i / (float) numberParallels)
					);

			helpQuaternion.RotateY(360.0f * texCoords[texCoords.size() - 1].x);
			helpMatrix = helpQuaternion.GetMatrix4();
			tangents.push_back(helpMatrix * helpVector);
		}
	}

	for (unsigned int i = 0; i < numberParallels; ++i) {
		for (unsigned int j = 0; j < numberSlices; ++j) {
			indices.push_back(i * (numberSlices + 1) + j);
			indices.push_back((i + 1) * (numberSlices + 1) + j);
			indices.push_back((i + 1) * (numberSlices + 1) + (j + 1));

			indices.push_back(i * (numberSlices + 1) + j);
			indices.push_back((i + 1) * (numberSlices + 1) + (j + 1));
			indices.push_back(i * (numberSlices + 1) + (j + 1));
		}
	}

	return true;
}

bool Sphere::InitializeVBO() {
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * vertices.size() * 3, &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * texCoords.size() * 2, &texCoords[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * normals.size() * 3, &normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &tangentBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * tangents.size() * 3, &tangents[0], GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	return true;
}

bool Sphere::InitializeVBA() {
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glBindVertexArray(0);

	return true;
}

void Sphere::Update(float deltaTime) { }

void Sphere::Render() {
	if (shader == NULL) {
		nge_log_error("Sphere --> Shader is NULL");
		return;
	} else if (texture == NULL) {
		nge_log_error("Sphere --> Texture is NULL");
		return;
	}

	shader->bindShader();
	texture->activate();

	shader->sendUniform4x4("modelview_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX));
	shader->sendUniform4x4("projection_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));
	shader->sendUniform("texture0", 0);

	//int verticesPerStrip = (slices + 1) * 2;
	//int start = 0;

	glBindVertexArray(vertexArray);
	/*for (int stack = 0; stack < stacks; ++stack)
	{
		glDrawArrays(GL_TRIANGLE_STRIP, start, (slices * 2) + 2);
		start += verticesPerStrip;
	}*/
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	shader->unbindShader();
	texture->deactivate();
}

void Sphere::SetShader(NGE::Media::Shaders::GLSLProgram * shader) {
	this->shader = shader;
}

void Sphere::SetTexture(NGE::Media::Images::Texture * texture) {
	this->texture = texture;
}