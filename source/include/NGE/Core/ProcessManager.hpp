/* 
 * File:   ProcessManager.hpp
 * Author: tku
 *
 * Created on 10 sierpień 2015, 03:16
 */

#ifndef PROCESSMANAGER_HPP
#define	PROCESSMANAGER_HPP

#include "NGE/Core/Process.hpp"

namespace NGE {
    namespace Core {

        class ProcessManager {
            typedef std::list<StrongProcessPtr> ProcessList;
            ProcessList processList;

          public:
            virtual ~ProcessManager();

            unsigned int UpdateProcesses(unsigned int deltaTime);
            WeakProcessPtr AttachProcess(StrongProcessPtr process);
            void AbortAllProcesses(bool immediate);

            unsigned int GetProcessCount() const;

          private:
            void ClearAllProcesses();
        };
    }
}

#endif	/* PROCESSMANAGER_HPP */

