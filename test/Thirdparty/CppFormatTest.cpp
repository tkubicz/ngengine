#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cppformat/format.h>

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
