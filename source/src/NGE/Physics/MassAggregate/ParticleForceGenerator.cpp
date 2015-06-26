#include <vector>

#include "NGE/Physics/MassAggregate/ParticleForceGenerator.hpp"

using namespace NGE::Physics::MassAggregate;

void ParticleForceRegistry::UpdateForces(float duration) {
	for (Registry::iterator i = registrations.begin(); i != registrations.end(); ++i) {
		i->fg->UpdateForce(i->particle, duration);
	}
}

void ParticleForceRegistry::Add(Particle* particle, ParticleForceGenerator* fg) {
	ParticleForceRegistry::ParticleForceRegistration registration;
	registration.particle = particle;
	registration.fg = fg;
	registrations.push_back(registration);
}

ParticleGravity::ParticleGravity(const NGE::Math::vec3f& gravity) : gravity(gravity) { }

void ParticleGravity::UpdateForce(Particle* particle, float duration) {
	if (!particle->HasFiniteMass()) return;

	particle->AddForce(gravity * particle->GetMass());
}

ParticleDrag::ParticleDrag(float k1, float k2) : k1(k1), k2(k2) { }

void ParticleDrag::UpdateForce(Particle* particle, float duration) {
	Math::vec3f force;
	particle->GetVelocity(&force);

	// Total drag coefficient
	float dragCoeff = force.Length();
	dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;

	// Calculate the final force and apply it
	force.Normalize();
	force *= -dragCoeff;
	particle->AddForce(force);
}

ParticleSpring::ParticleSpring(Particle* other, float springConstant, float restLength) : other(other), springConstant(springConstant), restLength(restLength) { }

void ParticleSpring::UpdateForce(Particle* particle, float duration) {
	Math::vec3f force;
	particle->GetPosition(&force);
	force -= other->GetPosition();

	float magnitude = force.Length();
	magnitude = std::abs(magnitude - restLength);
	magnitude *= springConstant;

	force.Normalize();
	force *= -magnitude;
	particle->AddForce(force);
}

ParticleAnchoredSpring::ParticleAnchoredSpring() { }

ParticleAnchoredSpring::ParticleAnchoredSpring(NGE::Math::vec3f* anchor, float springConstant, float restLength) : anchor(anchor), springConstant(springConstant), restLength(restLength) { }

void ParticleAnchoredSpring::Init(NGE::Math::vec3f* anchor, float springConstant, float restLength) {
	this->anchor = anchor;
	this->springConstant = springConstant;
	this->restLength = restLength;
}

void ParticleAnchoredSpring::UpdateForce(Particle* particle, float duration) {
	Math::vec3f force;
	particle->GetPosition(&force);
	force -= *anchor;

	float magnitude = force.Length();
	magnitude = (restLength - magnitude) * springConstant;

	force.Normalize();
	force *= magnitude;
	particle->AddForce(force);
}

ParticleBungee::ParticleBungee(Particle* other, float springConstant, float restLength) : other(other), springConstant(springConstant), restLength(restLength) { }

void ParticleBungee::UpdateForce(Particle* particle, float duration) {
	Math::vec3f force;
	particle->GetPosition(&force);
	force -= other->GetPosition();

	float magnitude = force.Length();
	if (magnitude <= restLength) return;

	magnitude = springConstant * (restLength - magnitude);

	force.Normalize();
	force *= -magnitude;
	particle->AddForce(force);
}

ParticleBuoyancy::ParticleBuoyancy(float maxDepth, float volume, float waterHeight, float liquidDensity) : maxDepth(maxDepth), volume(volume), waterHeight(waterHeight), liquidDensity(liquidDensity) { }

void ParticleBuoyancy::UpdateForce(Particle* particle, float duration) {
	float depth = particle->GetPosition().y;

	// Poza cieczą
	if (depth >= waterHeight + maxDepth) return;
	Math::vec3f force(0, 0, 0);

	// Maksymalna głębokość
	if (depth <= waterHeight - maxDepth) {
		force.y = liquidDensity * volume;
		particle->AddForce(force);
		return;
	}

	// Częsciowo zanurzone
	force.y = liquidDensity * volume * (depth - maxDepth - waterHeight) / 2 * maxDepth;
	particle->AddForce(force);
}

ParticleFakeSpring::ParticleFakeSpring(NGE::Math::vec3f* anchor, float springConstant, float damping) : anchor(anchor), springConstant(springConstant), damping(damping) { }

void ParticleFakeSpring::UpdateForce(Particle* particle, float duration) {
	if (!particle->HasFiniteMass()) return;

	Math::vec3f position;
	particle->GetPosition(&position);
	position -= *anchor;

	float gamma = 0.5f * sqrt(4 * springConstant - damping * damping);
	if (gamma == 0.0f) return;
	Math::vec3f c = position * (damping / (2.0f * gamma)) + particle->GetVelocity() * (1.0f / gamma);

	// Obliczenie końcowej pozycji
	Math::vec3f target = position * cos(gamma * duration) + c * sin(gamma * duration);
	target *= std::exp(-0.5f * duration * damping);

	Math::vec3f accel = (target - position) * (1.0f / duration * duration) - particle->GetVelocity() * duration;
	particle->AddForce(accel * particle->GetMass());
}

void ParticleAnchoredBungee::UpdateForce(Particle* particle, float duration) {
	Math::vec3f force;
	particle->GetPosition(&force);
	force -= *anchor;

	float magnitude = force.Length();
	if (magnitude < restLength)
		return;

	magnitude = magnitude - restLength;
	magnitude *= springConstant;

	force.Normalize();
	force *= -magnitude;
	particle->AddForce(force);
}
