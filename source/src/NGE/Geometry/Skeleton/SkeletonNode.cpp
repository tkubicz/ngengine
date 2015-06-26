#include "NGE/Geometry/Skeleton/SkeletonNode.hpp"
using namespace NGE::Geometry::Skeleton;

SkeletonNode::SkeletonNode(int parentIndex, float tx, float ty, float tz, int firstIndex, int lastIndex) {
	this->parent = NULL;
	t.Set(tx, ty, tz);
	angle.Set(0, 0, 0);
	this->firstIndex = firstIndex;
	this->lastIndex = lastIndex;
	this->parentIndex = parentIndex;

	matrix = new Math::mat4f();
	invMatrix = new Math::mat4f();

	UpdateMatrices();
}

SkeletonNode::SkeletonNode() {
	parent = NULL;
	t.Set(0, 0, 0);
	angle.Set(0, 0, 0);
	firstIndex = lastIndex = parentIndex = -1;
	matrix = invMatrix = NULL;
}

SkeletonNode::~SkeletonNode() {
	delete matrix;
	delete invMatrix;
}

void SkeletonNode::AddChild(SkeletonNode* node) {
	children.push_back(node);
	node->parent = this;
}

void SkeletonNode::RemoveChild(SkeletonNode* node) {
	children.remove(node);
	delete node;
}

int SkeletonNode::GetChildCount() const {
	return children.size();
}

SkeletonNode* SkeletonNode::GetParent() const {
	return parent;
}

void SkeletonNode::RotateX(float angleX) {
	angle.x = angleX;
	UpdateMatrices();
}

void SkeletonNode::RotateY(float angleY) {
	angle.y = angleY;
	UpdateMatrices();
}

void SkeletonNode::RotateZ(float angleZ) {
	angle.z = angleZ;
	UpdateMatrices();
}

void SkeletonNode::Translate(float tx, float ty, float tz) {
	t.Set(tx, ty, tz);
	UpdateMatrices();
}

void SkeletonNode::AttachVertices(int firstIndex, int lastIndex) {
	this->firstIndex = firstIndex;
	this->lastIndex = lastIndex;
}

void SkeletonNode::SetParentIndex(int parentIndex) {
	this->parentIndex = parentIndex;
}

int SkeletonNode::GetParentIndex() const {
	return parentIndex;
}

int SkeletonNode::GetFirstIndex() const {
	return firstIndex;
}

int SkeletonNode::GetLastIndex() const {
	return lastIndex;
}

void SkeletonNode::UpdateMatrices() {
	float toRad = (float) Math::MathUtils::PI / 180.0f;
	float cx = cos(angle.x * toRad);
	float sx = sin(angle.x * toRad);
	float cy = cos(angle.y * toRad);
	float sy = sin(angle.y * toRad);
	float cz = cos(angle.z * toRad);
	float sz = sin(angle.z * toRad);

	matrix->SetValueAt(0, cy * cz);
	matrix->SetValueAt(4, -cy * sz);
	matrix->SetValueAt(8, sy);
	matrix->SetValueAt(12, t.x);

	matrix->SetValueAt(1, sx * sy * cz + cx * sz);
	matrix->SetValueAt(5, -sx * sy * sz + cx * cz);
	matrix->SetValueAt(9, -sx * cy);
	matrix->SetValueAt(13, t.y);

	matrix->SetValueAt(2, -cx * sy * cz + sx * sz);
	matrix->SetValueAt(6, cx * sy * sz + sx * cz);
	matrix->SetValueAt(10, cx * cy);
	matrix->SetValueAt(14, t.z);

	matrix->SetValueAt(3, 0);
	matrix->SetValueAt(7, 0);
	matrix->SetValueAt(11, 0);
	matrix->SetValueAt(15, 1);

	*invMatrix = *matrix;
	invMatrix->SetInverse();
}

NGE::Math::mat4f* SkeletonNode::GetMatrix() const {
	return matrix;
}

NGE::Math::mat4f* SkeletonNode::GetInverseMatrix() const {
	return invMatrix;
}

std::vector<NGE::Math::vec3f> SkeletonNode::PreprocessPhase(std::vector<NGE::Math::vec3f>& vertices) {
	std::vector<Math::vec3f> verticesVT(vertices);
	Transform1(verticesVT, 0, 0, 0);

	return verticesVT;
}

std::vector<NGE::Math::vec3f> SkeletonNode::AnimationPhase(std::vector<NGE::Math::vec3f>& vertices) {
	Math::mat4f matrix;
	std::vector<Math::vec3f> verticesT(vertices);

	Transform2(verticesT, matrix);
	return verticesT;
}

void SkeletonNode::Transform1(std::vector<NGE::Math::vec3f>& vertices, float tx, float ty, float tz) {
	tx -= this->t.x;
	ty -= this->t.y;
	tz -= this->t.z;

	if (firstIndex > 0) {
		for (int i = firstIndex; i <= lastIndex; ++i) {
			vertices[i].x += tx;
			vertices[i].y += ty;
			vertices[i].z += tz;
		}
	}

	for (std::list<SkeletonNode*>::iterator i = children.begin(); i != children.end(); ++i)
		(*i)->Transform1(vertices, tx, ty, tz);
}

void SkeletonNode::Transform2(std::vector<NGE::Math::vec3f>& vertices, NGE::Math::mat4f matrix) {
	if (firstIndex > 0) {
		matrix *= *this->matrix;

		for (int i = firstIndex; i <= lastIndex; ++i) {
			vertices[i] = matrix * vertices[i];
		}
	}

	for (std::list<SkeletonNode*>::iterator i = children.begin(); i != children.end(); ++i) {
		(*i)->Transform2(vertices, matrix);
	}
}