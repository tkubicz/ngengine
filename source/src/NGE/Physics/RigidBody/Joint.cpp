#include "NGE/Physics/RigidBody/Joint.hpp"

using namespace NGE::Physics::RigidBody;

unsigned Joint::AddContact(Contact* contact, unsigned limit) const
{
    // Obliczenie pozycji każdego punktu połączenia w przestrzeni świata
    Math::vec3f aPosWorld = body[0]->GetPointInWorldSpace(position[0]);
    Math::vec3f bPosWorld = body[1]->GetPointInWorldSpace(position[1]);

    // Obliczenie długości połączenia
    Math::vec3f aToB = bPosWorld - aPosWorld;
    Math::vec3f normal = aToB;
    normal.Normalize();
    float length = aToB.Length();

    // Sprawdzenie czy naruszone
    if (std::abs(length) > error)
    {
        contact->body[0] = body[0];
        contact->body[1] = body[1];
        contact->contactNormal = normal;
        contact->contactPoint = (aPosWorld + bPosWorld) * 0.5f;
        contact->penetration = length - error;
        contact->friction = 1.0f;
        contact->restitution = 0;
        return 1;
    }

    return 0;
}

void Joint::Set(RigidBody* a, const NGE::Math::vec3f& aPos, RigidBody* b, const NGE::Math::vec3f& bPos, float error)
{
    body[0] = a;
    body[1] = b;

    position[0] = aPos;
    position[1] = bPos;

    this->error = error;
}


