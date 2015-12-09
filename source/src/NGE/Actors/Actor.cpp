#include "NGE/Actors/Actor.hpp"
using namespace NGE::Actors;

ActorId Actor::GetId() const {
    return id;
}

Actor::ActorType Actor::GetType() const {
    return type;
}

const Actor::ActorComponents* Actor::GetComponents() {
    return &components;
}



