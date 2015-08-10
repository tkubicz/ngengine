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

        // Process is uninitialized, so initialize it.
        if (currentProcess->GetState() == Process::UNINITIALIZED) {
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
            }

            // Remove process and destroy it.
            processList.erase(thisIt);
        }
    }

    return ((successCount << 16) | failCount);
}




