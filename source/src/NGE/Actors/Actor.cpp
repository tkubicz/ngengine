#include "NGE/Actors/Actor.hpp"
#include "NGE/Actors/ActorComponenet.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"
using namespace NGE::Actors;

// TODO: Use enum or const value instead of "unknown" string.

Actor::Actor(ActorId id) {
	this->id = id;
	this->type = "unknown";
	this->resource = "unknown";
}

Actor::~Actor() {
	log_debug("Destroying actor: '{}'", id);
	if (!components.empty()) {
		log_warn("Actor: '{}' was destroyed without destroying its components", id)
	}
}

bool Actor::Initialise(pugi::xml_node* data) {
	log_debug("Initialising Actor: '{}'" + id);
	type = data->attribute("type").as_string();
	resource = data->attribute("resource").as_string();
	return true;
}

void Actor::PostInit() {
	for (auto& kv : components) {
		kv.second->PostInit();
	}
}

ActorId Actor::GetId() const {
	return id;
}

void Actor::Terminate() {
	components.clear();
}

void Actor::Update(int deltaTimeMs) {
	for (auto& kv : components) {
		kv.second->Update(deltaTimeMs);
	}
}

std::string Actor::ToXml() {
	// TODO: Implement this method.
	return "";
}

Actor::ActorType Actor::GetType() const {
	return type;
}

const Actor::ActorComponents* Actor::GetComponents() {
	return &components;
}

void Actor::AddComponent(StrongActorComponentPtr componenet) {
	auto success = components.insert(std::make_pair(componenet->GetId(), componenet));
	if (!success.second) {
		log_warn("Error while adding component");
	}
}
