#include "NGE/Actors/ActorComponenet.hpp"
using namespace NGE::Actors;

ActorComponent::~ActorComponent() {
	owner.reset();
}

ComponentId ActorComponent::GetId() const {
	return GetIdFromName(GetName());
}

ComponentId ActorComponent::GetIdFromName(const std::string& componenetName) const {
	size_t hashedId = std::hash<std::string>()(componenetName);
	return hashedId;
}

void ActorComponent::SetOwner(StrongActorPtr owner) {
	this->owner = owner;
}
