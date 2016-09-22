#include "NGE/Rendering/Camera/TargetCamera.hpp"
#include "NGE/Math/Matrix4.hpp"
using namespace NGE::Rendering::Camera;

TargetCamera::TargetCamera() {
	right = Math::vec3f(1, 0, 0);
	up = Math::vec3f(0, 1, 0);
	look = Math::vec3f(0, 0, -1);
	minRy = -60;
	maxRy = 60;
	minDistance = 1;
	maxDistance = 10;
}

TargetCamera::~TargetCamera() {
}

void TargetCamera::Update() {
	//Math::mat4f r = 
}

void TargetCamera::Rotate(const float yaw, const float pitch, const float roll) {
	this->yaw += yaw;
	this->pitch += pitch;
	this->pitch = std::min(std::max(this->pitch, minRy), maxRy);
	Update();
}
