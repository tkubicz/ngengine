#include "NGE/Physics/RigidBody/World.hpp"
using namespace NGE::Physics::RigidBody;

World::World(unsigned maxContacts, unsigned iterations) : resolver(iterations), maxContacts(maxContacts) {
	contacts = new Contact[maxContacts];
	calculateIterations = (iterations == 0);
}

World::~World() {
	delete[] contacts;
}

void World::StartFrame() {
	BodyRegistration* reg = firstBody;
	while (reg) {
		// Remove all forces from the accumulator.
		reg->body->ClearAccumulators();
		reg->body->CalculateDerivedData();

		// Get the next registration.
		reg = reg->next;
	}
}

unsigned World::GenerateContacts() {
	unsigned limit = maxContacts;
	Contact* nextContact = contacts;

	ContactGenRegistration* reg = firstContactGen;
	while (reg) {
		unsigned used = reg->gen->AddContact(nextContact, limit);
		limit -= used;
		nextContact += used;

		// We've run out of contacts to fill. This means we're missing contacts.
		if (limit <= 0)
			break;

		reg = reg->next;
	}

	// Return the number of contacts used
	return maxContacts - limit;
}

void World::RunPhysics(float duration) {
	// First apply the force generators
	// TODO: Fix this code
	//registry.UpdateForces(duration);

	// Then integrate the objects.
	BodyRegistration* reg = firstBody;
	while (reg) {
		// Remove all forces from the accumulator
		reg->body->Integrate(duration);

		// Get the next registration
		reg = reg->next;
	}

	// Generate contacts
	unsigned usedContacts = GenerateContacts();

	// And process them.
	if (calculateIterations)
		resolver.SetIterations(usedContacts * 4);

	resolver.ResolveContacts(contacts, usedContacts, duration);
}
