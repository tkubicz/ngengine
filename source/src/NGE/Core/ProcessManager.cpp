#include <list>
#include "NGE/Core/ProcessManager.hpp"
using namespace NGE::Core;

ProcessManager::~ProcessManager() {
	ClearAllProcesses();
}

unsigned int ProcessManager::UpdateProcesses(unsigned int deltaTime) {
	unsigned short int successCount = 0;
	unsigned short int failCount = 0;

	ProcessList::iterator it = processList.begin();
	while (it != processList.end()) {
		// Grab the next process.
		StrongProcessPtr currentProcess = (*it);

		// Save the iterator and increment the old one in case we need to
		// remove this process from list.
		ProcessList::iterator thisIt = it;
		++it;

		// Process is uninitialised, so initialise it.
		if (currentProcess->GetState() == Process::UNINITIALISED) {
			currentProcess->OnInit();
		}

		// Give the process an update tick if it's running.
		if (currentProcess->GetState() == Process::RUNNING) {
			currentProcess->OnUpdate(deltaTime);
		}

		// Check to see if the process is dead.
		if (currentProcess->IsDead()) {
			// Run the appropriate exit function.
			switch (currentProcess->GetState()) {
				case Process::SUCCEEDED:
				{
					currentProcess->OnSuccess();
					StrongProcessPtr child = currentProcess->RemoveChild();
					if (child) {
						AttachProcess(child);
					} else {
						// Only counts if the whole chain is completed.
						++successCount;
					}
					break;
				}

				case Process::FAILED:
				{
					currentProcess->OnFail();
					++failCount;
					break;
				}

				case Process::ABORTED:
				{
					currentProcess->OnAbort();
					++failCount;
					break;
				}

				default:
					break;
			}

			// Remove process and destroy it.
			processList.erase(thisIt);
		}
	}

	return ((successCount << 16) | failCount);
}

WeakProcessPtr ProcessManager::AttachProcess(NGE::Core::StrongProcessPtr process) {
	processList.push_front(process);
	return WeakProcessPtr(process);
}

void ProcessManager::ClearAllProcesses() {
	processList.clear();
}

void ProcessManager::AbortAllProcesses(bool immediate) {
	ProcessList::iterator it = processList.begin();
	while (it != processList.end()) {
		ProcessList::iterator tempIt = it;
		++it;

		StrongProcessPtr process = *tempIt;
		if (process->IsAlive()) {
			process->SetState(Process::ABORTED);
			if (immediate) {
				process->OnAbort();
				processList.erase(tempIt);
			}
		}
	}
}

unsigned int ProcessManager::GetProcessCount() const {
	return processList.size();
}
