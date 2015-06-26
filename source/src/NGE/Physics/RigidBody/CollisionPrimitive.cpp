#include "NGE/Physics/RigidBody/CollisionPrimitive.hpp"
using namespace NGE::Physics::RigidBody;

void CollisionPrimitive::CalculateInternals() {
	transform = body->GetTransform() * offset;
}

NGE::Math::vec3f CollisionPrimitive::GetAxis(unsigned index) const {
	return transform.GetAxisVector(index);
}

const NGE::Math::mat4f& CollisionPrimitive::GetTransform() const {
	return transform;
}




