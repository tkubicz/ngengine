#include "catch.hpp"
#include "pugixml.hpp"
#include "NGE/Events/EventManager.hpp"
#include "NGE/Media/Images/TextureManager.hpp"
#include "NGE/Media/Images/LoadTextureEvent.hpp"

namespace i = NGE::Media::Images;
namespace e = NGE::Events;

using namespace pugi;

SCENARIO("Initialise TextureManager", "[texture-manager]") {

	GIVEN("New TextureManager") {
		i::TextureManager& tm = i::TextureManager::GetInstance();

		WHEN("TextureManager is initialised") {
			tm.Initialise();

			THEN("Global Event manager should have listeners registered") {
				e::EventManager& em = e::EventManager::GetInstance();
				auto allListeners = em[em.GLOBAL]->ListAllListeners();
				for (auto& kv : allListeners) {
					REQUIRE((kv.first == "load-texture" || kv.first == "get-texture"));
				}
			}
		}
	}
}

SCENARIO("Listen for events", "[texture-manager]") {

	GIVEN("TextureManager") {
		i::TextureManager& tm = i::TextureManager::GetInstance();
		tm.Initialise();

		GIVEN("XML texture configuration") {
			pugi::xml_document doc;

			WHEN("LoadTextureEvent is sent") {
			}
		}
	}
}