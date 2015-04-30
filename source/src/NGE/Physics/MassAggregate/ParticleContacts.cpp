#include <float.h>

#include "NGE/Physics/MassAggregate/ParticleContacts.hpp"
#include "NGE/Physics/MassAggregate/Particle.hpp"

using namespace NGE::Physics::MassAggregate;

void ParticleContact::Resolve(float duration)
{
    ResolveVelocity(duration);
    ResolveInterpenetration(duration);
}

float ParticleContact::CalculateSeperatingVelocity() const
{
    Math::vec3f relativeVelocity = particle[0]->GetVelocity();
    if (particle[1])
    {
        relativeVelocity -= particle[1]->GetVelocity();
    }
    return relativeVelocity.DotProduct(contactNormal);
}

void ParticleContact::ResolveVelocity(float duration)
{
    // Znalezienie prędkości w kierunku kontaktu
    float seperatingVelocity = CalculateSeperatingVelocity();

    // Sprawdzenie czy wymagane jest rozwiązanie
    if (seperatingVelocity > 0)
    {
        // Obiekty albo się oddalają, albo stoją w miejscu
        return;
    }

    // Obliczenie nowej prędkości oddalania się
    float newSepVelocity = -seperatingVelocity * restitution;

    Math::vec3f accCausedVelocity = particle[0]->GetAcceleration();
    if (particle[1]) accCausedVelocity -= particle[1]->GetAcceleration();
    float accCausedSepVelocity = accCausedVelocity.DotProduct(contactNormal) * duration;

    if (accCausedSepVelocity < 0)
    {
        newSepVelocity += restitution * accCausedSepVelocity;

        if (newSepVelocity < 0)
            newSepVelocity = 0;
    }

    float deltaVelocity = newSepVelocity - seperatingVelocity;

    float totalInverseMass = particle[0]->GetInverseMass();
    if (particle[1])
        totalInverseMass += particle[1]->GetInverseMass();

    if (totalInverseMass <= 0)
        return;

    float impulse = deltaVelocity / totalInverseMass;

    Math::vec3f impulsePerIMass = contactNormal * impulse;

    particle[0]->SetVelocity(particle[0]->GetVelocity() + impulsePerIMass * particle[0]->GetInverseMass());

    if (particle[1])
    {
        particle[1]->SetVelocity(particle[1]->GetVelocity() + impulsePerIMass * -particle[1]->GetInverseMass());
    }
}

void ParticleContact::ResolveInterpenetration(float duration)
{
    if (penetration <= 0)
        return;

    float totalInverseMass = particle[0]->GetInverseMass();
    if (particle[1])
        totalInverseMass += particle[1]->GetInverseMass();

    if (totalInverseMass <= 0)
        return;

    Math::vec3f movePerImass = contactNormal * (penetration / totalInverseMass);

    particleMovment[0] = movePerImass * particle[0]->GetInverseMass();

    if (particle[1])
        particleMovment[1] = movePerImass * -particle[1]->GetInverseMass();
    else
        particleMovment[1].Clear();

    particle[0]->SetPosition(particle[0]->GetPosition() + particleMovment[0]);

    if (particle[1])
        particle[1]->SetPosition(particle[1]->GetPosition() + particleMovment[1]);
}

ParticleContactResolver::ParticleContactResolver(unsigned iterations) : iterations(iterations) { }

void ParticleContactResolver::SetIterations(unsigned iterations)
{
    this->iterations = iterations;
}

void ParticleContactResolver::ResolveContacts(ParticleContact* contactArray, unsigned numContacts, float duration)
{
    unsigned i;
    iterationsUsed = 0;

    while (iterationsUsed < iterations)
    {
        // Znalezienie kontaktu z największą predkością zbliżania się
        float max = FLT_MAX;
        unsigned maxIndex = numContacts;
        for (i = 0; i < numContacts; ++i)
        {
            float sepVel = contactArray[i].CalculateSeperatingVelocity();
            if (sepVel < max && (sepVel < 0 || contactArray[i].penetration > 0))
            {
                max = sepVel;
                maxIndex = i;
            }
        }

        if (maxIndex == numContacts) break;

        contactArray[maxIndex].Resolve(duration);

        Math::vec3f* move = contactArray[maxIndex].particleMovment;
        for (i = 0; i < numContacts; ++i)
        {
            if (contactArray[i].particle[0] == contactArray[maxIndex].particle[0])
                contactArray[i].penetration -= move[0].DotProduct(contactArray[i].contactNormal);
            else if (contactArray[i].particle[0] == contactArray[maxIndex].particle[1])
                contactArray[i].penetration -= move[1].DotProduct(contactArray[i].contactNormal);

            if (contactArray[i].particle[1])
            {
                if (contactArray[i].particle[1] == contactArray[maxIndex].particle[0])
                    contactArray[i].penetration += move[0].DotProduct(contactArray[i].contactNormal);
                else if (contactArray[i].particle[1] == contactArray[maxIndex].particle[1])
                    contactArray[i].penetration += move[1].DotProduct(contactArray[i].contactNormal);
            }
        }

        iterationsUsed++;
    }
}
