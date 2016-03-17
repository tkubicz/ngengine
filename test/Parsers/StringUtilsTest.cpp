#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "NGE/Parsers/StringUtils.hpp"

BOOST_AUTO_TEST_CASE(ReplaceOneStringToAnother) {
	std::string exampleString = "This is example string";
	std::string expectedString = "This is quite longer string";

	NGE::Parsers::StringUtils::Replace(exampleString, "example", "quite longer");
	BOOST_CHECK_EQUAL(expectedString, exampleString);
}

BOOST_AUTO_TEST_CASE(ReplaceStringThatDoesntExist) {
	std::string exampleString = "This is an example string";
	std::string expectedString = exampleString;

	NGE::Parsers::StringUtils::Replace(exampleString, "test", "test2");
	BOOST_CHECK_EQUAL(expectedString, exampleString);
}

BOOST_AUTO_TEST_CASE(ReplaceMultipleStrings) {
	std::string exampleString = "This is some example string which needs to be replaced to another string";
	std::string expectedString = "This is some example text which needs to be replaced to another text";

	NGE::Parsers::StringUtils::ReplaceAll(exampleString, "string", "text");
	BOOST_CHECK_EQUAL(expectedString, exampleString);
}

BOOST_AUTO_TEST_CASE(ReplaceMultipleStringThatDoesntExist) {
	std::string exampleString = "This is some example string which needs to be replaced to another string";
	std::string expectedString = exampleString;

	NGE::Parsers::StringUtils::ReplaceAll(exampleString, "test", "test2");
	BOOST_CHECK_EQUAL(expectedString, exampleString);
}