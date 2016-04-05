#include "catch.hpp"
#include "NGE/Media/MediaManager.hpp"

namespace m = NGE::Media;

SCENARIO("Initialise MediaManager") {

	GIVEN("MediaManager reference") {
		m::MediaManager& mediaManager = m::MediaManager::GetInstance();

		WHEN("MediaManager is initialised") {
			mediaManager.initialize();

			THEN("MediaManager should be initialised") {
			}
		}
	}
}