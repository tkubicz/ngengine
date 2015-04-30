#include "NGE/Physics/MassAggregate/ParticleLinks.hpp"

using namespace NGE::Physics::MassAggregate;

float ParticleLink::currentLength() const
{
    Math::vec3f relativePos = particle[0]->GetPosition() - particle[1]->GetPosition();
    return relativePos.Length();
}

unsigned ParticleCable::AddContact(ParticleContact* contact, unsigned limit) const
{
    // Znalezienie długości kabla
    float lenght = currentLength();

    // Sprawdzenie czy kabel został zbytnio rozciągnięty
    if (lenght < maxLength)
        return 0;

    // W przeciwnym wypadku zwracamy kontakt
    contact->particle[0] = particle[0];
    contact->particle[1] = particle[1];

    // Obliczenie normalnej
    Math::vec3f normal = particle[1]->GetPosition() - particle[0]->GetPosition();
    normal.Normalize();
    contact->contactNormal = normal;

    contact->penetration = lenght - maxLength;
    contact->restitution = restitution;

    return 1;
}

unsigned ParticleRod::AddContact(ParticleContact* contact, unsigned limit) const
{
    // Znalezienie długości prętu
    float currentLen = currentLength();

    if (currentLen == length)
        return 0;

    contact->particle[0] = particle[0];
    contact->particle[1] = particle[1];

    Math::vec3f normal = particle[1]->GetPosition() - particle[0]->GetPosition();
    normal.Normalize();

    // Normalna kontaktu zależy od tego czy rozszerzamy czy ściskamy pręt
    if (currentLen > length)
    {
        contact->contactNormal = normal;
        contact->penetration = currentLength() - length;
    }
    else
    {
        contact->contactNormal = normal * -1;
        contact->penetration = length - currentLength();
    }

    contact->restitution = 0;
    return 1;
}

float ParticleConstraint::currentLength() const
{
    Math::vec3f relativePos = particle->GetPosition() - anchor;
    return relativePos.Length();
}

unsigned ParticleCableConstraint::AddContact(ParticleContact* contact, unsigned limit) const
{
    float length = currentLength();

    if (length < maxLength)
        return 0;

    contact->particle[0] = particle;
    contact->particle[1] = 0;

    Math::vec3f normal = anchor - particle->GetPosition();
    normal.Normalize();
    contact->contactNormal = normal;

    contact->penetration = length - maxLength;
    contact->restitution = restitution;

    return 1;
}

unsigned ParticleRodConstraint::AddContact(ParticleContact* contact, unsigned limit) const
{
    float currentLen = currentLength();

    if (currentLen == length)
        return 0;

    contact->particle[0] = particle;
    contact->particle[1] = 0;

    Math::vec3f normal = anchor - particle->GetPosition();
    normal.Normalize();

    if (currentLen > length)
    {
        contact->contactNormal = normal;
        contact->penetration = currentLen - length;
    }
    else
    {
        contact->contactNormal = normal * -1;
        contact->penetration = length - currentLen;
    }

    contact->restitution = 0;

    return 1;
}
