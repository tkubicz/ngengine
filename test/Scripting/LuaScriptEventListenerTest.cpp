#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "NGE/ThirdPart/selene.h"
#include "NGE/Scripting/LuaScriptEventListener.hpp"

using namespace sel;
using namespace NGE::Scripting;

BOOST_AUTO_TEST_CASE(Test) {
    // Create lua context and load standard library.
    State state{true};
}
