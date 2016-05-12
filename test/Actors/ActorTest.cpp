#include <catch.hpp>
#include <functional>
#include "NGE/Actors/Actor.hpp"

namespace a = NGE::Actors;

SCENARIO("Actor creation", "[actor]") {

	WHEN("Actor is created") {
		//a::Actor* actor = new a::Actor(0);
	}
}

SCENARIO("Hash test", "[actor]") {

	GIVEN("Some example string") {
		std::string str1 = "This is some example string";
		const char* str2 = "This is some example string";

		WHEN("String is hashed") {
			std::size_t hash1 = std::hash<std::string>()(str1);

			WHEN("String is hashed again") {
				std::size_t hash2 = std::hash<std::string>()(str2);

				THEN("Hashes are equal") {
					REQUIRE(hash1 == hash2);
				}
			}
		}
	}
}