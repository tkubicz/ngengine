#include "NGE/Physics/RigidBody/IntersectionTests.hpp"

using namespace NGE::Physics::RigidBody;

bool IntersectionTests::SphereAndHalfSpace(const CollisionSphere& sphere, const CollisionPlane& plane)
{
    // Odległość od punktu początkowego.
    float ballDistance = plane.direction.DotProduct(sphere.GetAxis(3)) - sphere.radius;

    // Sprawdzenie, czy mamy przecięcie.
    return ballDistance <= plane.offset;
}

bool IntersectionTests::SphereAndSphere(const CollisionSphere& one, const CollisionSphere& two)
{
    // Znalezienie wektora pomiędzy obiektami.
    Math::vec3f midline = one.GetAxis(3) - two.GetAxis(3);

    // Sprawdzenie, czy wektor jest wystarczająco duży.
    return midline.LengthSquared() < (one.radius + two.radius) * (one.radius + two.radius);
}

float IntersectionTests::TransformToAxis(const CollisionBox& box, const NGE::Math::vec3f& axis)
{
    return box.halfSize.x * std::abs(axis.DotProduct(box.GetAxis(0))) +
        box.halfSize.y * std::abs(axis.DotProduct(box.GetAxis(1))) +
        box.halfSize.z * std::abs(axis.DotProduct(box.GetAxis(2)));
}

float IntersectionTests::OverlapOnAxis(const CollisionBox& one, const CollisionBox& two, const NGE::Math::vec3f& axis, const NGE::Math::vec3f& toCenter)
{
    float oneProject = TransformToAxis(one, axis);
    float twoProject = TransformToAxis(two, axis);

    float distance = std::abs(toCenter.DotProduct(axis));
    
    return (distance < oneProject + twoProject);
}

bool IntersectionTests::BoxAndBox(const CollisionBox& one, const CollisionBox& two) {
    // Znalezienie wektora pomiędzy środkami
    Math::vec3f toCenter = two.GetAxis(3) - one.GetAxis(3);
    
    return (
        OverlapOnAxis(one, two, one.GetAxis(0), toCenter) &&
        OverlapOnAxis(one, two, one.GetAxis(1), toCenter) &&
        OverlapOnAxis(one, two, one.GetAxis(2), toCenter) &&
        
        OverlapOnAxis(one, two, two.GetAxis(0), toCenter) &&
        OverlapOnAxis(one, two, two.GetAxis(1), toCenter) &&
        OverlapOnAxis(one, two, two.GetAxis(2), toCenter) &&
        
        OverlapOnAxis(one, two, one.GetAxis(0).CrossProduct(two.GetAxis(0)), toCenter) &&
        OverlapOnAxis(one, two, one.GetAxis(0).CrossProduct(two.GetAxis(1)), toCenter) &&
        OverlapOnAxis(one, two, one.GetAxis(0).CrossProduct(two.GetAxis(2)), toCenter) &&
        OverlapOnAxis(one, two, one.GetAxis(1).CrossProduct(two.GetAxis(0)), toCenter) &&
        OverlapOnAxis(one, two, one.GetAxis(1).CrossProduct(two.GetAxis(1)), toCenter) &&
        OverlapOnAxis(one, two, one.GetAxis(1).CrossProduct(two.GetAxis(2)), toCenter) &&
        OverlapOnAxis(one, two, one.GetAxis(2).CrossProduct(two.GetAxis(0)), toCenter) &&
        OverlapOnAxis(one, two, one.GetAxis(2).CrossProduct(two.GetAxis(1)), toCenter) &&
        OverlapOnAxis(one, two,one.GetAxis(2).CrossProduct(two.GetAxis(2)), toCenter)
        );
}

bool IntersectionTests::BoxAndHalfSpace(const CollisionBox& box, const CollisionPlane& plane) {
    float projectedRadius = TransformToAxis(box, plane.direction);
    
    float boxDistance = plane.direction.DotProduct(box.GetAxis(3)) - projectedRadius;
    
    return boxDistance <= plane.offset;
 }