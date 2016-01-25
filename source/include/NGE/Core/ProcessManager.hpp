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

			/**
			 * The process update tick. Called every logic tick. This function returns
			 * the number of process chains that succeeded in the upper 32 bits and
			 * the number of process chains that failed or were aborted in the lower
			 * 32 bits.
			 * @param deltaTime 
			 * @return 
			 */
			unsigned int UpdateProcesses(unsigned int deltaTime);
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

