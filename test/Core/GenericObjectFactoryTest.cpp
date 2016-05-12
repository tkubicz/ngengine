#include <catch.hpp>
#include "NGE/Core/GenericObjectFactory.hpp"
#include "NGE/Actors/ActorComponenet.hpp"

namespace c = NGE::Core;
namespace a = NGE::Actors;

SCENARIO("GenericObjectFactory usage", "[generic-object-factory]") {

	GIVEN("New instance") {
		c::GenericObjectFactory<a::ActorComponent, a::ComponentId> componenetFactory;

		WHEN("Classes are registered") {
			//componenetFactory.Register()
		}
	}
}