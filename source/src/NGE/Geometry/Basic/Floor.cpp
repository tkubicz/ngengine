#include "NGE/Geometry/Basic/Floor.hpp"
#include "NGE/Rendering/Renderer.hpp"
#include "NGE/Core/Core.hpp"
using namespace NGE::Geometry::Basic;

Floor::Floor() {
	color.Set(0.0f, 0.6f, 0.0f, 1.0f);
}

Floor::~Floor() { }

bool Floor::Initialize() {
	return Initialize(100.f);
}

bool Floor::Initialize(float halfSize, float step, NGE::Media::Shaders::GLSLProgram* shader) {
	this->halfSize = halfSize;
	this->step = step;
	this->shader = shader;

	if (InitializeGeometry() && InitializeVBO() && InitializeVBA())
		return true;

	return false;
}

bool Floor::InitializeGeometry() {
	// Fill the vertices vector.
	for (float i = -halfSize; i <= halfSize; i += step) {
		vertices.push_back(Math::vec3f(-halfSize, 0.f, i));
		vertices.push_back(Math::vec3f(halfSize, 0.f, i));
		vertices.push_back(Math::vec3f(i, 0.f, -halfSize));
		vertices.push_back(Math::vec3f(i, 0.f, halfSize));
	}

	return true;
}

bool Floor::InitializeVBO() {
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * vertices.size() * 3, &vertices[0], GL_STATIC_DRAW);

	return true;
}

bool Floor::InitializeVBA() {
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	return true;
}

void Floor::Update(float dt) { }

void Floor::Render() {
	Rendering::Renderer::GetInstance().GetMatrixStack().PushMatrix();
	shader->BindShader();

	shader->sendUniform4x4("modelview_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX));
	shader->sendUniform4x4("projection_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));
	shader->sendUniform("color", color);

	glBindVertexArray(vertexArray);
	glDrawArrays(GL_LINES, 0, vertices.size());
	glBindVertexArray(0);

	shader->UnbindShader();
	Rendering::Renderer::GetInstance().GetMatrixStack().PopMatrix();
}

void Floor::SetColor(const NGE::Math::vec4f& color) {
	if (shader == NULL)
		return;

	this->color = color;
	/*shader->BindShader();
	shader->SendUniform("color", color);
	shader->UnbindShader();*/
}

void Floor::DeleteBuffers() {
	if (vertexBuffer != 0)
		glDeleteBuffers(1, &vertexBuffer);
}

void Floor::DeleteArrays() {
	if (vertexArray != 0)
		glDeleteVertexArrays(1, &vertexArray);
}

