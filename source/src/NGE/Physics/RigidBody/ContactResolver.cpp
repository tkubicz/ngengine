#include "NGE/Physics/RigidBody/ContactResolver.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"

using namespace NGE::Physics::RigidBody;

ContactResolver::ContactResolver(unsigned iterations, float velocityEpsilon, float positionEpsilon) {
    SetIterations(iterations, iterations);
    SetEpsilon(velocityEpsilon, positionEpsilon);
}

ContactResolver::ContactResolver(unsigned velocityIterations, unsigned positionIterations, float velocityEpsilon, float positionEpsilon) {
    SetIterations(velocityIterations);
    SetEpsilon(velocityEpsilon, positionEpsilon);
}

bool ContactResolver::IsValid() {
    return (velocityIterations > 0) &&
            (positionIterations > 0) &&
            (positionEpsilon > 0.0f) &&
            (velocityEpsilon > 0.0f);
}

void ContactResolver::SetIterations(unsigned iterations) {
    SetIterations(iterations, iterations);
}

void ContactResolver::SetIterations(unsigned velocityIterations, unsigned positionIterations) {
    this->velocityIterations = velocityIterations;
    this->positionIterations = positionIterations;
}

void ContactResolver::SetEpsilon(float velocityEpsilon, float positionEpsilon) {
    this->velocityEpsilon = velocityEpsilon;
    this->positionEpsilon = positionEpsilon;
}

void ContactResolver::ResolveContacts(Contact* contactArray, unsigned numContacts, float duration) {
    // Upewnienie się, że jest coś do zrobienia
    if (numContacts == 0)
        return;
    if (!IsValid())
        return;

    // Przygotowanie kontaktów do przetworzenia
    PrepareContacts(contactArray, numContacts, duration);

    AdjustPosition(contactArray, numContacts, duration);

    AdjustVelocities(contactArray, numContacts, duration);
}

void ContactResolver::PrepareContacts(Contact* contactArray, unsigned numContacts, float duration) {
    Contact* lastContact = contactArray + numContacts;
    for (Contact* contact = contactArray; contact < lastContact; contact++) {
        contact->CalculateInternals(duration);
    }
}

void ContactResolver::AdjustVelocities(Contact* contactArray, unsigned numContacts, float duration) {
    Math::vec3f velocityChange[2], rotationChange[2];
    Math::vec3f deltaVel;

    velocityIterationsUsed = 0;
    while (velocityIterationsUsed < velocityIterations) {
        float max = velocityEpsilon;
        unsigned index = numContacts;
        for (unsigned i = 0; i < numContacts; ++i) {
            if (contactArray[i].desiredDeltaVelocity > max) {
                max = contactArray[i].desiredDeltaVelocity;
                index = i;
            }
        }
        if (index == numContacts)
            break;

        contactArray[index].MatchAwakeState();

        contactArray[index].ApplyVelocityChange(velocityChange, rotationChange);

        for (unsigned i = 0; i < numContacts; ++i) {
            for (unsigned b = 0; b < 2; ++b) {
                if (contactArray[i].body[b]) {
                    for (unsigned d = 0; d < 2; d++) {
                        if (contactArray[i].body[b] == contactArray[index].body[d]) {
                            deltaVel = velocityChange[d] + rotationChange[d].CrossProduct(contactArray[i].relativeContactPosition[b]);

                            contactArray[i].contactVelocity += contactArray[i].contactToWorld.TransformTranspose(deltaVel) * (b ? -1.0f : 1.0f);

                            contactArray[i].CalculateDesiredDeltaVelocity(duration);
                        }
                    }
                }
            }
        }
        velocityIterationsUsed++;
    }
}

void ContactResolver::AdjustPosition(Contact* contacts, unsigned numContacts, float duration) {
    unsigned i, index;
    Math::vec3f linearChange[2], angularChange[2];
    float max;
    Math::vec3f deltaPosition;

    positionIterationsUsed = 0;
    while (positionIterationsUsed < positionIterations) {
        // Znalezienie największej penetracji
        max = positionEpsilon;
        index = numContacts;
        for (i = 0; i < numContacts; ++i) {
            if (contacts[i].penetration > max) {
                max = contacts[i].penetration;
                index = i;
            }
        }

        if (index == numContacts)
            break;

        contacts[index].MatchAwakeState();

        contacts[index].ApplyPositionChange(linearChange, angularChange, max);

        for (i = 0; i < numContacts; ++i) {
            for (unsigned b = 0; b < 2; ++b) {
                if (contacts[i].body[b]) {
                    for (unsigned d = 0; d < 2; ++d) {
                        if (contacts[i].body[b] == contacts[index].body[d]) {
                            deltaPosition = linearChange[d] + angularChange[d].CrossProduct(contacts[i].relativeContactPosition[b]);

                            contacts[i].penetration += deltaPosition.DotProduct(contacts[i].contactNormal) * (b ? 1 : -1);
                        }
                    }
                }
            }
        }
        positionIterationsUsed++;
    }
}








