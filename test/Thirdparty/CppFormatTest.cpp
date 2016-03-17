#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <chrono>
#include <boost/test/unit_test.hpp>
#include <cppformat/format.h>
#include <iomanip>
#include "NGE/Parsers/StringUtils.hpp"

namespace ch = std::chrono;

#define BATC(name) BOOST_AUTO_TEST_CASE(name)

BOOST_AUTO_TEST_CASE(InitialTest) {
	std::string testString = fmt::format("{0} {1} {2} {0}", "test", 1, 20.45f);
	BOOST_CHECK_EQUAL("test 1 20.45 test", testString);
}

BOOST_AUTO_TEST_CASE(MemoryWriter) {
	fmt::MemoryWriter w;
	w.write("Look, a {} string", "C");
	BOOST_CHECK_EQUAL("Look, a C string", w.c_str());
}

BATC(PrintTest) {
	fmt::print(stderr, "System error code = {}\n", errno);
	fmt::print("Don't {}\n", "panic");
}

template<typename... Args> std::string testFunc(const std::string& format, Args&&... arguments) {
	return fmt::format(format, std::forward<Args>(arguments)...);
}

BATC(VariadicTemplateTest) {
	std::string expectedStr = "This is sparta!!!";
	std::string resultStr = testFunc("{} {} {}{}", "This", "is", "sparta", "!!!");
	BOOST_CHECK_EQUAL(expectedStr, resultStr);
}

BATC(MemoryWriterAndPrintTest) {
	fmt::MemoryWriter mw;
	mw.write("{first} {second} {third}", fmt::arg("first", "This"), fmt::arg("second", "is"), fmt::arg("third", "Sparta!"), fmt::arg("forth", "1"));
	BOOST_CHECK_EQUAL("This is Sparta!", mw.c_str());
}

BATC(MemoryWriterTimeTest) {
	ch::high_resolution_clock::time_point timePoint = ch::high_resolution_clock::now();
	auto milliseconds = ch::duration_cast<ch::milliseconds>(timePoint.time_since_epoch());
	auto seconds = ch::duration_cast<ch::seconds>(milliseconds);
	std::time_t currentTime = seconds.count();
	std::tm localTime = *std::localtime(&currentTime);
	unsigned short fractional_seconds = milliseconds.count() % 1000;
	std::string format = "%Y-%m-%d %H:%M:%S.%f";
	NGE::Parsers::StringUtils::ReplaceAll(format, "%f", std::to_string(fractional_seconds));
	
	//std::cout << std::put_time(&localTime, format.c_str()) << std::endl;

	char* timeBuffer = new char[100];
	if (std::strftime(&timeBuffer[0], 100, format.c_str(), &localTime)) {
		std::string result = std::string(&timeBuffer[0]);
		std::cout << std::string(&timeBuffer[0]) << std::endl;
		std::cout << "result size: " << result.size() << std::endl;
	} else {
		std::cout << "error?" << std::endl;
	}
}