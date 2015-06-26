#include "NGE/Physics/RigidBody/BoundingSphere.hpp"
#include "NGE/Math/MathUtils.hpp"

using namespace NGE::Physics::RigidBody;

BoundingSphere::BoundingSphere(const NGE::Math::vec3f& center, float radius) {
	this->center = center;
	this->radius = radius;
}

BoundingSphere::BoundingSphere(const BoundingSphere& one, const BoundingSphere& two) {
	Math::vec3f centerOffset = two.center - one.center;
	float distance = centerOffset.LengthSquared();
	float radiusDiff = two.radius - one.radius;

	// Sprawdzenie czy większa sfera zawiera mniejszą
	if (radiusDiff * radiusDiff >= distance) {
		if (one.radius > two.radius) {
			center = one.center;
			radius = one.radius;
		} else {
			center = two.center;
			radius = two.radius;
		}
	}		// W innym wypadku rozważenie częściowo nakładających się sfer
	else {
		distance = std::sqrt(distance);
		radius = (distance + one.radius + two.radius) * (float) 0.5f;

		// Nowy środek jest oparty na środku pierwszej sfery, przesuniętym
		// w kierunku środka drugiej sfery przez wartość proporcjonalną do
		// promieni sfer.
		center = one.center;
		if (distance > 0) {
			center += centerOffset * ((radius - one.radius) / distance);
		}
	}
}

bool BoundingSphere::Overlaps(const BoundingSphere* other) const {
	float distanceSquared = (center - other->center).LengthSquared();
	return distanceSquared < (radius + other->radius) * (radius + other->radius);
}

float BoundingSphere::GetGrowth(const BoundingSphere& other) const {
	BoundingSphere newSphere(*this, other);

	// Zwracamy wartość proporcjonalną do zmiany w powierzchni sfery.
	return newSphere.radius * newSphere.radius - radius * radius;
}

float BoundingSphere::GetSize() const {
	return ((float) 1.333333) * (float) Math::MathUtils::PI * radius * radius * radius;
}




