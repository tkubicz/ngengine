#include "catch.hpp"
#include "NGE/Core/Process.hpp"
using namespace NGE::Core;

class TestProcess : public Process {
  public:

	virtual void OnUpdate(unsigned int deltaTime) {
	}

};

class TestProcessWrapper : public TestProcess {
  public:
	using TestProcess::OnInit;
	using TestProcess::SetState;

};

SCENARIO("Changing process states", "[process]") {

	GIVEN("Newly created Process") {
		TestProcessWrapper process;
		REQUIRE(process.GetState() == Process::State::UNINITIALISED);

		WHEN("Process is initialised") {
			process.OnInit();

			THEN("Process should be in state 'RUNNING'") {
				REQUIRE(process.GetState() == Process::State::RUNNING);
			}

			AND_WHEN("Process is successfully finished") {
				process.Succeed();

				THEN("Process should be in state 'SUCCEEDED'") {
					REQUIRE(process.GetState() == Process::State::SUCCEEDED);
				}
			}

			AND_WHEN("Process fails") {
				process.Fail();

				THEN("Process should be in state 'FAILED'") {
					REQUIRE(process.GetState() == Process::State::FAILED);
				}
			}

			AND_WHEN("Process is paused") {
				process.Pause();

				THEN("Process should be in state 'PAUSED'") {
					REQUIRE(process.GetState() == Process::State::PAUSED);
				}

				AND_WHEN("Process is run from pause") {
					process.Run();

					THEN("Process should be in state 'RUNNING'") {
						REQUIRE(process.GetState() == Process::State::RUNNING);
					}
				}
			}
		}
	}
}

void checkState(const Process& process, bool alive, bool dead, bool removed, bool paused) {
	REQUIRE(process.IsAlive() == alive);
	REQUIRE(process.IsDead() == dead);
	REQUIRE(process.IsRemoved() == removed);
	REQUIRE(process.IsPaused() == paused);
}

SCENARIO("Get process state", "[process]") {

	GIVEN("Newly created process") {
		TestProcessWrapper process;

		WHEN("Process is in state 'UNINITALISED'") {
			checkState(process, false, false, false, false);
		}

		WHEN("Process is in state 'RUNNING'") {
			process.SetState(Process::State::RUNNING);
			checkState(process, true, false, false, false);
		}

		WHEN("Process is in state 'PAUSED'") {
			process.SetState(Process::State::PAUSED);
			checkState(process, true, false, false, true);
		}

		WHEN("Process is in state 'SUCCEEDED'") {
			process.SetState(Process::State::SUCCEEDED);
			checkState(process, false, true, false, false);
		}

		WHEN("Process is in state 'FAILED'") {
			process.SetState(Process::State::FAILED);
			checkState(process, false, true, false, false);
		}

		WHEN("Process is in state 'ABORTED'") {
			process.SetState(Process::State::ABORTED);
			checkState(process, false, true, false, false);
		}

		WHEN("Process is in state 'REMOVED'") {
			process.SetState(Process::State::REMOVED);
			checkState(process, false, false, true, false);
		}
	}
}