#include "NGE/Rendering/Camera/FreeCamera.hpp"
#include "NGE/Math/Matrix4.hpp"
using namespace NGE::Rendering::Camera;

FreeCamera::FreeCamera() {
	translation = Math::vec3f(0, 0, 0);
}

FreeCamera::~FreeCamera() {
}

void FreeCamera::Update() {
	Math::mat4f R;
	R.RotateXYZ(yaw, pitch, roll);
	position += translation;

	// Set this when no movement decay is needed.
	translation.Clear();

	look = Math::vec3f(R * Math::vec4f(0, 0, 1, 0));
	up = Math::vec3f(R * Math::vec4f(0, 1, 0, 0));
	right = look.CrossProduct(up);

	Math::vec3f target = position + look;
	viewMatrix.LookAt(position, target, up);
}

void FreeCamera::Walk(const float dt) {
	translation += (look * speed * dt);
	Update();
}

void FreeCamera::Strafe(const float dt) {
	translation += (right * speed * dt);
	Update();
}

void FreeCamera::Lift(const float dt) {
	translation += (up * speed * dt);
	Update();
}

float FreeCamera::GetSpeed() const {
	return speed;
}

void FreeCamera::SetSpeed(float speed) {
	this->speed = speed;
}

NGE::Math::vec3f FreeCamera::GetTranslation() const {
	return translation;
}

void FreeCamera::SetTranslation(Math::vec3f translation) {
	this->translation = translation;
}