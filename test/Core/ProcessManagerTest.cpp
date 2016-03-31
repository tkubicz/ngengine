#include "catch.hpp"
#include "NGE/Core/Process.hpp"
#include "NGE/Core/ProcessManager.hpp"
using namespace NGE::Core;

class TestProcess : public Process {
  public:

	unsigned int timeSinceStart;
	bool onSuccessExecuted;
	bool onFailExecuted;
	bool onAbortExecuted;

	TestProcess() : Process() {
		timeSinceStart = 0;
		onSuccessExecuted = onFailExecuted = onAbortExecuted = false;
	}

	void SetState(State state) {
		Process::SetState(state);
	}

	virtual void OnUpdate(unsigned int deltaTime) override {
		timeSinceStart += deltaTime;
	}

	virtual void OnSuccess() override {
		onSuccessExecuted = true;
	}

	virtual void OnFail() override {
		onFailExecuted = true;
	}

	virtual void OnAbort() override {
		onAbortExecuted = true;
	}
};

class ChildProcess : public Process {
  public:

	std::string processName;

	ChildProcess(const std::string& processName) : Process(), processName(processName) {
	}
};

SCENARIO("Attach process to process manager", "[process-manager][process]") {

	GIVEN("Process manager and process instances") {
		ProcessManager manager;
		StrongProcessPtr process = std::make_shared<Process>();

		WHEN("Process is attached to process manager") {
			REQUIRE(manager.GetProcessCount() == 0);
			WeakProcessPtr weakProcessPtr = manager.AttachProcess(process);

			THEN("Weak process pointer is returned and process is attached") {
				REQUIRE_FALSE(weakProcessPtr.expired());
				REQUIRE(weakProcessPtr.use_count() == 2);
				REQUIRE(manager.GetProcessCount() == 1);
			}
		}
	}
}

SCENARIO("Update processes", "[process-manager][process]") {

	GIVEN("Process manager with attached process") {
		ProcessManager manager;
		StrongProcessPtr process = std::make_shared<TestProcess>();
		WeakProcessPtr weakProcess = manager.AttachProcess(process);
		REQUIRE_FALSE(weakProcess.expired());

		WHEN("Process manager is updated") {
			manager.UpdateProcesses(10);

			THEN("Process should be in state 'RUNNING'") {
				REQUIRE(weakProcess.lock()->GetState() == Process::State::RUNNING);
				REQUIRE(((TestProcess*) weakProcess.lock().get())->timeSinceStart == 10);

				WHEN("Process state is set to 'SUCCEEDED'") {
					((TestProcess*) weakProcess.lock().get())->SetState(Process::State::SUCCEEDED);
					manager.UpdateProcesses(10);

					THEN("Process manager should execute OnSuccess() method from process") {
						REQUIRE(weakProcess.lock()->GetState() == Process::State::SUCCEEDED);
						REQUIRE(((TestProcess*) weakProcess.lock().get())->timeSinceStart == 10);
						REQUIRE(((TestProcess*) weakProcess.lock().get())->onSuccessExecuted == true);

						THEN("Process should be removed") {
							REQUIRE(manager.GetProcessCount() == 0);
						}
					}
				}

				WHEN("Process state is set to 'SUCCEEDED' and process has child") {
					weakProcess.lock()->AttachChild(std::make_shared<ChildProcess>("child-process-name"));
					((TestProcess*) weakProcess.lock().get())->SetState(Process::State::SUCCEEDED);
					manager.UpdateProcesses(10);

					THEN("Process manager should execute OnSuccess() method from process") {
						REQUIRE(weakProcess.lock()->GetState() == Process::State::SUCCEEDED);
						REQUIRE(((TestProcess*) weakProcess.lock().get())->timeSinceStart == 10);
						REQUIRE(((TestProcess*) weakProcess.lock().get())->onSuccessExecuted == true);

						THEN("Process count should stay the same because child process should replace parent") {
							REQUIRE(manager.GetProcessCount() == 1);
						}
					}
				}

				WHEN("Process state is set to 'FAILED'") {
					((TestProcess*) weakProcess.lock().get())->SetState(Process::State::FAILED);
					manager.UpdateProcesses(10);

					THEN("Process manager should execute OnFail() method from process") {
						REQUIRE(weakProcess.lock()->GetState() == Process::State::FAILED);
						REQUIRE(((TestProcess*) weakProcess.lock().get())->timeSinceStart == 10);
						REQUIRE(((TestProcess*) weakProcess.lock().get())->onFailExecuted == true);

						THEN("Process should be removed") {
							REQUIRE(manager.GetProcessCount() == 0);
						}
					}
				}

				WHEN("Process state is set to 'ABORTED'") {
					((TestProcess*) weakProcess.lock().get())->SetState(Process::State::ABORTED);
					manager.UpdateProcesses(10);

					THEN("Process manager should execute OnAbort() method from process") {
						REQUIRE(weakProcess.lock()->GetState() == Process::State::ABORTED);
						REQUIRE(((TestProcess*) weakProcess.lock().get())->timeSinceStart == 10);
						REQUIRE(((TestProcess*) weakProcess.lock().get())->onAbortExecuted == true);

						THEN("Process should be removed") {
							REQUIRE(manager.GetProcessCount() == 0);
						}
					}
				}
			}
		}
	}
}

SCENARIO("Abort all processes", "[process-manager][process]") {

	GIVEN("Process manager with processes running") {
		ProcessManager manager;
		const size_t processCount = 10;

		std::array<StrongProcessPtr, processCount> processes;
		for (StrongProcessPtr& process : processes) {
			process = std::make_shared<TestProcess>();
			manager.AttachProcess(process);
		}

		REQUIRE(manager.GetProcessCount() == processCount);
		manager.UpdateProcesses(10);

		for (StrongProcessPtr& process : processes) {
			REQUIRE(process->GetState() == Process::State::RUNNING);
		}

		WHEN("Abort all processes") {
			manager.AbortAllProcesses(false);

			THEN("All processes should be in state aborted") {
				for (StrongProcessPtr& process : processes) {
					REQUIRE(process->GetState() == Process::State::ABORTED);
				}
				REQUIRE(manager.GetProcessCount() == processCount);

				AND_THEN("After update all processes should be removed from managed") {
					manager.UpdateProcesses(10);
					REQUIRE(manager.GetProcessCount() == 0);

					AND_THEN("All OnAbort() methods should be executed") {
						for (StrongProcessPtr& process : processes) {
							REQUIRE(((TestProcess*) process.get())->onAbortExecuted == true);
						}
					}
				}
			}
		}

		WHEN("Abort all processes immediately") {
			manager.AbortAllProcesses(true);

			THEN("All processes should be in state aborted") {
				for (StrongProcessPtr& process : processes) {
					REQUIRE(process->GetState() == Process::State::ABORTED);
				}

				AND_THEN("All processes should be removed from manager") {
					REQUIRE(manager.GetProcessCount() == 0);
				}

				AND_THEN("All OnAbort() methods should be executed") {
					for (StrongProcessPtr& process : processes) {
						REQUIRE(((TestProcess*) process.get())->onAbortExecuted == true);
					}
				}
			}
		}
	}
}