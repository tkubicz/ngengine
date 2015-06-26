#include <fstream>
#include "NGE/Geometry/Skeleton/SkinnedMesh.hpp"
#include "NGE/Geometry/Skeleton/SkeletonNode.hpp"
#include "NGE/Tools/Logger.hpp"
#include "NGE/Rendering/Renderer.hpp"
#include "NGE/Core/Core.hpp"
using namespace NGE::Geometry::Skeleton;

SkinnedMesh::SkinnedMesh(PolyType type) {
	polytype = type;
	skeleton = NULL;
	shader = NULL;
}

SkinnedMesh::~SkinnedMesh() { }

void SkinnedMesh::LoadMesh(const std::string& filename) {
	float vx, vy, vz;
	int numVerts, numPolys;
	int ivert1, ivert2, ivert3, ivert4;

	Math::vec3f* dummy = new Math::vec3f();
	verticesV.push_back(dummy);

	std::ifstream ifile(filename.c_str(), std::ios::in);
	if (!ifile.is_open()) {
		Tools::Logger::WriteErrorLog("SkinnedMesh --> Could not open file: " + filename);
		return;
	}

	ifile >> numVerts >> numPolys;

	for (int i = 0; i < numVerts; ++i) {
		ifile >> vx >> vy >> vz;
		Math::vec3f* p = new Math::vec3f(vx, vy, vz);
		verticesV.push_back(p);
	}

	for (int i = 0; i < numPolys; ++i) {
		ifile >> ivert1 >> ivert2 >> ivert3;
		if (polytype == SkinnedMesh::QUAD)
			ifile >> ivert4;
		else
			ivert4 = 0;

		Math::vec4i* poly = new Math::vec4i();
		poly->Set(ivert1, ivert2, ivert3, ivert4);
		polygons.push_back(poly);
	}

	ifile.close();

	// Tymczasowe utworzenie dodatkowej tablicy wierzchołĸów -- kod do przepisania
	for (unsigned i = 0; i < verticesV.size(); ++i) {
		vertices.push_back(Math::vec3f(*(verticesV[i])));
		vertices2.push_back(Math::vec3f(*(verticesV[i])));
	}

	for (unsigned i = 0; i < polygons.size(); ++i) {
		indices.push_back(polygons[i]->x);
		indices.push_back(polygons[i]->y);
		indices.push_back(polygons[i]->z);
		indices.push_back(polygons[i]->w);
	}


	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 3 * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArray);

	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer((GLint) 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glBindVertexArray(0);
}

void SkinnedMesh::Render() {
	if (shader == NULL)
		return;

	shader->BindShader();
	shader->SendUniform4x4("modelview_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX));
	shader->SendUniform4x4("projection_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));
	shader->SendUniform("color", Math::vec3f(1.0, 0.0, 0.0));

	if (skeleton != NULL)
		Transform();

	glBindVertexArray(vertexArray);
	// TODO: Fix that, it is not going to work on opengl es.
#ifndef ANDROID
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_INT, 0);
#endif
	glBindVertexArray(0);
}

void SkinnedMesh::SetColor(float r, float g, float b) {
	color.Set(r, g, b);
}

void SkinnedMesh::SetColor(const NGE::Math::vec3f& color) {
	this->color = color;
}

void SkinnedMesh::Normal(const NGE::Math::vec3f* p1, const NGE::Math::vec3f* p2, const NGE::Math::vec3f* p3) const {
	// TODO: zrobić liczenie normalnych i przerzucanie ich do wektora
}

void SkinnedMesh::AttachSkeleton(Skeleton* skeleton) {
	this->skeleton = skeleton;
	Preprocess();
}

Skeleton* SkinnedMesh::GetSkeleton() const {
	return skeleton;
}

void SkinnedMesh::Preprocess() {
	SkeletonNode* root = skeleton->GetRoot();
	verticesVT = root->PreprocessPhase(vertices);
}

void SkinnedMesh::Transform() {
	SkeletonNode* root = skeleton->GetRoot();
	verticesW = root->AnimationPhase(verticesVT);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, verticesW.size() * 3 * sizeof (GLfloat), &verticesW[0]);
}

void SkinnedMesh::SetShader(NGE::Media::Shaders::GLSLProgram* shader) {
	this->shader = shader;
}

void SkinnedMesh::DeleteBuffers() {
	if (vertexBuffer != 0)
		glDeleteBuffers(1, &vertexBuffer);

	if (indexBuffer != 0)
		glDeleteBuffers(1, &normalBuffer);
}

void SkinnedMesh::DeleteArrays() {
	if (vertexArray != 0)
		glDeleteVertexArrays(1, &vertexArray);
}