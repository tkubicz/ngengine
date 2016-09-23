#include "NGE/Rendering/Camera/FreeCamera.hpp"
#include "NGE/Math/Matrix4.hpp"
using namespace NGE::Rendering::Camera;

FreeCamera::FreeCamera() {
	translation = Math::vec3f(0, 0, 0);
}

FreeCamera::~FreeCamera() {
}

void FreeCamera::Update() {
}

void FreeCamera::Rotate(const float yaw, const float pitch, const float roll) {

}



