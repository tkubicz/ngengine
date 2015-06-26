#include <vector>
#include "NGE/Physics/MassAggregate/ParticleWorld.hpp"

using namespace NGE::Physics::MassAggregate;

ParticleWorld::ParticleWorld(unsigned maxContacts, unsigned iterations) : resolver(iterations), maxContacts(maxContacts) {
	contacts = new ParticleContact[maxContacts];
	calculateIterations = (iterations == 0);
}

ParticleWorld::~ParticleWorld() {
	delete[] contacts;
}

void ParticleWorld::StartFrame() {
	for (Particles::iterator p = particles.begin(); p != particles.end(); ++p)
		(*p)->ClearAccumulator();
}

unsigned ParticleWorld::GenerateContacts() {
	unsigned limit = maxContacts;
	ParticleContact* nextContact = contacts;

	for (ContactGenerators::iterator g = contactGenerators.begin(); g != contactGenerators.end(); ++g) {
		unsigned used = (*g)->AddContact(nextContact, limit);
		limit -= used;
		nextContact += used;

		if (limit <= 0)
			break;
	}

	return maxContacts - limit;
}

void ParticleWorld::Integrate(float duration) {
	for (Particles::iterator p = particles.begin(); p != particles.end(); ++p)
		(*p)->Integrate(duration);
}

void ParticleWorld::RunPhysics(float duration) {
	registry.UpdateForces(duration);

	Integrate(duration);

	unsigned usedContacts = GenerateContacts();

	if (usedContacts) {
		if (calculateIterations)
			resolver.SetIterations(usedContacts * 2);
		resolver.ResolveContacts(contacts, usedContacts, duration);
	}
}

ParticleWorld::Particles& ParticleWorld::GetParticles() {
	return particles;
}

ParticleWorld::ContactGenerators& ParticleWorld::GetContactGenerators() {
	return contactGenerators;
}

ParticleForceRegistry& ParticleWorld::GetForceRegistry() {
	return registry;
}

void GroundContacts::Init(ParticleWorld::Particles* particles) {
	this->particles = particles;
}

unsigned GroundContacts::AddContact(ParticleContact* contact, unsigned limit) const {
	unsigned count = 0;
	for (ParticleWorld::Particles::iterator p = particles->begin(); p != particles->end(); p++) {
		float y = (*p)->GetPosition().y;

		if (y < 0.0f) {
			contact->contactNormal = Math::vec3f(0.0f, 1.0f, 0.0f); // vec3f UP
			contact->particle[0] = *p;
			contact->particle[1] = NULL;
			contact->penetration = -y;
			contact->restitution = 0.2f;
			contact++;
			count++;
		}

		if (count >= limit)
			return count;
	}

	return count;
}
