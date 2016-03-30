#include <iostream>
#include "catch.hpp"
#include "NGE/Parsers/StringUtils.hpp"

namespace s = NGE::Parsers;

SCENARIO("Replace strings", "[string-utils]") {

	GIVEN("Example string: 'This is example string'") {
		std::string exampleString = "This is example string";

		WHEN("Replace is applied: 'example -> quite longer'") {
			std::string expectedString = "This is quite longer string";
			s::StringUtils::Replace(exampleString, "example", "quite longer");

			THEN("Substring should be replaced") {
				REQUIRE(exampleString == expectedString);
			}
		}

		WHEN("Replace is applied: 'test -> test2' (doesn't exist)") {
			std::string expectedString = exampleString;
			s::StringUtils::Replace(exampleString, "test", "test2");

			THEN("String should not be altered") {
				REQUIRE(exampleString == expectedString);
			}
		}
	}

	GIVEN("Example string: 'This is some example string which needs to be replaced to another string'") {
		std::string exampleString = "This is some example string which needs to be replaced to another string";

		WHEN("Replace is applied: 'string -> text'") {
			std::string expectedString = "This is some example text which needs to be replaced to another text";
			s::StringUtils::ReplaceAll(exampleString, "string", "text");

			THEN("Strings should be replaced") {
				REQUIRE(exampleString == expectedString);
			}
		}

		WHEN("Replace is applied: 'test -> test2' (doesn't exist)") {
			std::string expectedString = exampleString;
			s::StringUtils::ReplaceAll(exampleString, "test", "test2");

			THEN("String should not be altered") {
				REQUIRE(exampleString == expectedString);
			}
		}
	}
}

SCENARIO("Generate random strings", "[string-utils]") {

	WHEN("Random string is requested") {
		std::string randomString = s::StringUtils::RandomString(10);

		THEN("Random string is generated") {
			REQUIRE(randomString.size() == 10);
		}
	}

	WHEN("Two random strings are requested") {
		std::srand(std::time(0));
		std::string randomString1 = s::StringUtils::RandomString(15);
		std::string randomString2 = s::StringUtils::RandomString(15);

		THEN("Random strings are generated and not equal") {
			REQUIRE((randomString1.size() == 15 && randomString2.size() == 15));
			REQUIRE(randomString1 != randomString2);
		}
	}
}