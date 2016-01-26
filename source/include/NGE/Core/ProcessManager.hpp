/* 
 * File:   ProcessManager.hpp
 * Author: tku
 *
 * Created on 10 sierpień 2015, 03:16
 */

#ifndef PROCESSMANAGER_HPP
#define PROCESSMANAGER_HPP

#include "NGE/Core/Process.hpp"
#include <list>

namespace NGE {
	namespace Core {

		class ProcessManager {
		  private:
			typedef std::list<NGE::Core::StrongProcessPtr> ProcessList;
			typedef std::list<NGE::Core::Process*> RawProcessList;

			ProcessList processList;
			RawProcessList rawProcessList;

		  public:
			virtual ~ProcessManager();

			unsigned int UpdateProcesses(unsigned int deltaTime) {
				unsigned int strongProcessResult = UpdateProcesses(deltaTime, processList);
				unsigned int rawProcessResult = UpdateProcesses(deltaTime, rawProcessList);
			}
			
			/**
			 * The process update tick. Called every logic tick. This function returns
			 * the number of process chains that succeeded in the upper 32 bits and
			 * the number of process chains that failed or were aborted in the lower
			 * 32 bits.
			 * @param deltaTime 
			 * @return 
			 */
			template <typename T> unsigned int UpdateProcesses(unsigned int deltaTime, std::list<T>& list) {
				unsigned short int successCount = 0;
				unsigned short int failCount = 0;

				auto it = list.begin();
				while (it != list.end()) {
					// Grab the next process.
					T currentProcess = (*it);

					// Save the iterator and increment the old one in case we need to
					// remove this process from list.
					auto thisIt = it;
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
						list.erase(thisIt);
					}
				}

				return ((successCount << 16) | failCount);
			}

			WeakProcessPtr AttachProcess(NGE::Core::StrongProcessPtr process);

			template <typename T> void AbortAllProcesses(bool immediate, std::list<T>& list) {
				auto it = list.begin();
				while (it != processList.end()) {
					auto tempIt = it;
					++it;

					T process = *tempIt;
					if (process->IsAlive()) {
						process->SetState(Process::ABORTED);
						if (immediate) {
							process->OnAbort();
							list.erase(tempIt);
						}
					}
				}
			}

			unsigned int GetProcessCount() const;

		  private:
			void ClearAllProcesses();
		};
	}
}

#endif /* PROCESSMANAGER_HPP */

