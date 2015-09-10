/* 
 * File:   IScriptManager.hpp
 * Author: tku
 *
 * Created on 11 wrzesień 2015, 01:32
 */

#ifndef ISCRIPTMANAGER_HPP
#define	ISCRIPTMANAGER_HPP

namespace NGE {
    namespace Scripting {

        class IScriptManager {
          public:

            virtual ~IScriptManager() { }
            virtual bool Init() = 0;
            virtual void ExecuteFile(const std::string& resource) = 0;
            virtual void ExecuteString(const std::string& str) = 0;
        };
    }
}

#endif	/* ISCRIPTMANAGER_HPP */

