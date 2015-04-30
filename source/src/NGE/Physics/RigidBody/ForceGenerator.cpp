#include "NGE/Physics/RigidBody/ForceGenerator.hpp"

using namespace NGE::Physics::RigidBody;

Gravity::Gravity(const NGE::Math::vec3f& gravity)
{
    this->gravity = gravity;
}

void Gravity::UpdateForce(RigidBody* body, float duration)
{
    // Sprawdzenie czy obiekt nie ma nieskończonej masy
    if (!body->HasFiniteMass())
        return;

    body->AddForce(gravity * body->GetMass());
}

Spring::Spring(const NGE::Math::vec3f& locationConnectionPoint, RigidBody* other, const NGE::Math::vec3f& otherConnectionPoint, float springConstant, float restLength)
{
    this->connectionPoint = locationConnectionPoint;
    this->other = other;
    this->otherConnectionPoint = otherConnectionPoint;
    this->springConstant = springConstant;
    this->restLength = restLength;
}

void Spring::UpdateForce(RigidBody* body, float duration)
{
    // Obliczenie dwóch końców w przestrzeni świata
    Math::vec3f lws = body->GetPointInWorldSpace(connectionPoint);
    Math::vec3f ows = body->GetPointInWorldSpace(otherConnectionPoint);

    // Obliczenie wektora
    Math::vec3f force = lws - ows;

    // Obliczenie długości siły
    float magnitude = force.Length();
    magnitude = std::abs(magnitude - restLength);
    magnitude *= springConstant;

    // Obliczenie siły i zastosowanie jej
    force.Normalize();
    force *= magnitude;
    body->AddForceAtPoint(force, lws);
}

Aero::Aero(const NGE::Math::mat3f& tensor, const NGE::Math::vec3f& position, const NGE::Math::vec3f* windspeed)
{
    this->tensor = tensor;
    this->position = position;
    this->windspeed = windspeed;
}

void Aero::UpdateForce(RigidBody* body, float duration)
{
    UpdateForceFromTensor(body, duration, tensor);
}

void Aero::UpdateForceFromTensor(RigidBody* body, float duration, const NGE::Math::mat3f& tensor)
{
    // Obliczenie całkowitej prędkości
    Math::vec3f velocity = body->GetVelocity();
    velocity += *windspeed;

    // Obliczenie prędkości we współrzędnych ciała
    Math::vec3f bodyVel = body->GetTransform().TransformInverseDirection(velocity);

    // Obliczenie siły we współrzędnych ciała
    Math::vec3f bodyForce = tensor * bodyVel;
    Math::vec3f force = body->GetTransform().TransformDirection(bodyForce);

    // Zastosowanie siły
    body->AddForceAtBodyPoint(force, position);
}

AeroControl::AeroControl(const NGE::Math::mat3f& base, const NGE::Math::mat3f& min, const NGE::Math::mat3f& max, const NGE::Math::vec3f& position, const NGE::Math::vec3f* windspeed) :
Aero(base, position, windspeed)
{
    this->minTensor = min;
    this->maxTensor = max;
    controlSetting = 0.0f;
}

NGE::Math::mat3f AeroControl::GetTensor()
{
    if (controlSetting <= -1.0f)
        return minTensor;
    else if (controlSetting >= 1.0f)
        return maxTensor;
    else if (controlSetting < 0)
        return minTensor.LinearInterpolate(tensor, controlSetting + 1.0f);
    else if (controlSetting > 0)
        return tensor.LinearInterpolate(maxTensor, controlSetting);
    else
        return tensor;
}

void AeroControl::SetControl(float value)
{
    controlSetting = value;
}

void AeroControl::UpdateForce(RigidBody* body, float duration)
{
    Math::mat3f tensor = GetTensor();
    Aero::UpdateForceFromTensor(body, duration, tensor);
}

void ForceRegistry::UpdateForces(float duration)
{
    for (Registry::iterator i = registrations.begin(); i != registrations.end(); ++i)
    {
        i->fg->UpdateForce(i->body, duration);
    }
}

void ForceRegistry::Add(RigidBody* body, ForceGenerator* fg)
{
    ForceRegistry::ForceRegistration registration;
    registration.body = body;
    registration.fg = fg;
    registrations.push_back(registration);
}
