#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <memory>
#include <boost/test/unit_test.hpp>
#include "NGE/Core/Process.hpp"
using namespace NGE::Core;

class TestProcess : public Process {
  public:

    virtual void OnUpdate(unsigned int deltaTime) { }

};

class TestProcessWrapper : public TestProcess {
  public:
    using TestProcess::OnInit;
    using TestProcess::SetState;

};

BOOST_AUTO_TEST_CASE(CreateProcess) {
    TestProcess process;
    BOOST_CHECK_EQUAL(Process::UNINITIALIZED, process.GetState());
}

BOOST_AUTO_TEST_CASE(OnInit) {
    TestProcessWrapper process;
    process.OnInit();
    BOOST_CHECK_EQUAL(Process::RUNNING, process.GetState());
}

BOOST_AUTO_TEST_CASE(Success) {
    TestProcessWrapper process;
    process.OnInit();

    process.Succeed();
    BOOST_CHECK_EQUAL(Process::SUCCEEDED, process.GetState());

    process.SetState(Process::FAILED);
    process.Succeed();
    BOOST_CHECK_EQUAL(Process::FAILED, process.GetState());
}

BOOST_AUTO_TEST_CASE(Fail) {
    TestProcessWrapper process;
    
    process.SetState(Process::PAUSED);
    process.Fail();
    BOOST_CHECK_EQUAL(Process::FAILED, process.GetState());
    
    process.SetState(Process::ABORTED);
    process.Fail();
    BOOST_CHECK_EQUAL(Process::ABORTED, process.GetState());
}