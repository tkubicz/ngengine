#include "NGE/Rendering/Camera/AbstractCamera.hpp"
using namespace NGE::Rendering::Camera;

NGE::Math::vec3f NGE::Rendering::Camera::AbstractCamera::UP = Math::vec3f(0, 1, 0);

AbstractCamera::AbstractCamera() {
	zNear = 0.1f;
	zFar = 1000.f;
}

AbstractCamera::~AbstractCamera() {
}

void AbstractCamera::SetupProjection(const float fovY, const float aspectRatio, const float near, const float far) {
	projectionMatrix.SetPerspectiveProjection(fovY, aspectRatio, near, far);
	zNear = near;
	zFar = far;
	fov = fovY;
	this->aspectRatio = aspectRatio;
}

const NGE::Math::mat4f AbstractCamera::GetViewMatrix() const {
	return viewMatrix;
}

const NGE::Math::mat4f AbstractCamera::GetProjectionMatrix() const {
	return projectionMatrix;
}

const NGE::Math::vec3f AbstractCamera::GetPosition() const {
	return position;
}

void AbstractCamera::SetPosition(const Math::vec3f& p) {
	position = p;
}

const float AbstractCamera::GetFOV() const {
	return fov;
}

void AbstractCamera::SetFOV(const float fov) {
	this->fov = fov;
	projectionMatrix.SetPerspectiveProjection(fov, aspectRatio, zNear, zFar);
}

const float AbstractCamera::GetAspectRatio() const {
	return aspectRatio;
}

void AbstractCamera::CalculateFrustumPlanes() {
	Math::vec3f cN = position + look * zNear;
	Math::vec3f cF = position + look * zFar;
}


void AbstractCamera::Rotate(const float yaw, const float pitch, const float roll) {
	this->yaw = Math::MathUtils::DegToRad(yaw);
	this->pitch = Math::MathUtils::DegToRad(pitch);
	this->roll = Math::MathUtils::DegToRad(roll);
	Update();
}






