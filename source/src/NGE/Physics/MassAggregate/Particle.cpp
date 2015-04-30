#include <assert.h>
#include <float.h>

#include "NGE/Physics/MassAggregate/Particle.hpp"

using namespace NGE::Physics::MassAggregate;

void Particle::Integrate(float duration)
{
    // Nie całkujemy rzeczy z masą zero.
    if (inverseMass <= 0.0f) return;

    assert(duration > 0.0);

    // Aktualizacja liniowej pozycji
    position += velocity * duration;

    // Przyspieszenie z mocy
    Math::vec3f resultingAcc = acceleration;
    resultingAcc += forceAccum * inverseMass;

    // Aktualizacja liniowej prędkości z przyspieszenia
    velocity += resultingAcc * duration;

    // Tarcie
    velocity *= pow(damping, duration);

    // Wyczyszczenie mocy
    ClearAccumulator();
}

void Particle::SetMass(const float mass)
{
    assert(mass != 0);
    this->inverseMass = (float) 1.0 / mass;
}

float Particle::GetMass() const
{
    if (inverseMass == 0)
        return FLT_MAX;
    else
        return (float) 1.0 / inverseMass;
}

void Particle::SetInverseMass(const float inverseMass)
{
    this->inverseMass = inverseMass;
}

float Particle::GetInverseMass() const
{
    return inverseMass;
}

bool Particle::HasFiniteMass() const
{
    return inverseMass >= 0.0f;
}

void Particle::SetDamping(const float damping)
{
    this->damping = damping;
}

float Particle::GetDamping() const
{
    return damping;
}

void Particle::SetPosition(const NGE::Math::vec3f& position)
{
    this->position = position;
}

void Particle::SetPosition(const float x, const float y, const float z)
{
    position.Set(x, y, z);
}

void Particle::GetPosition(NGE::Math::vec3f* position) const
{
    *position = this->position;
}

NGE::Math::vec3f Particle::GetPosition() const
{
    return position;
}

void Particle::SetVelocity(const NGE::Math::vec3f& velocity)
{
    this->velocity = velocity;
}

void Particle::SetVelocity(const float x, const float y, const float z)
{
    velocity.Set(x, y, z);
}

void Particle::GetVelocity(NGE::Math::vec3f* velocity) const
{
    *velocity = this->velocity;
}

NGE::Math::vec3f Particle::GetVelocity() const
{
    return velocity;
}

void Particle::SetAcceleration(const NGE::Math::vec3f& acceleration)
{
    this->acceleration = acceleration;
}

void Particle::SetAcceleration(const float x, const float y, const float z)
{
    acceleration.Set(x, y, z);
}

void Particle::GetAcceleration(NGE::Math::vec3f* acceleration) const
{
    *acceleration = this->acceleration;
}

NGE::Math::vec3f Particle::GetAcceleration() const
{
    return acceleration;
}

void Particle::ClearAccumulator()
{
    forceAccum.Clear();
}

void Particle::AddForce(const NGE::Math::vec3f& force)
{
    forceAccum += force;
}
