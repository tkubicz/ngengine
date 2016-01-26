#include <list>

#include "NGE/Core/ProcessManager.hpp"
using namespace NGE::Core;

ProcessManager::~ProcessManager() {
	ClearAllProcesses();
}

WeakProcessPtr ProcessManager::AttachProcess(NGE::Core::StrongProcessPtr process) {
	processList.push_front(process);
	return WeakProcessPtr(process);
}

void ProcessManager::ClearAllProcesses() {
	processList.clear();
}

unsigned int ProcessManager::GetProcessCount() const {
	return processList.size();
}
