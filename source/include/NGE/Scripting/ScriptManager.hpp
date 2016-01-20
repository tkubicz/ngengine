/* 
 * File:   IScriptManager.hpp
 * Author: tku
 *
 * Created on 11 September 2015, 01:32
 */

#ifndef ISCRIPTMANAGER_HPP
#define ISCRIPTMANAGER_HPP

namespace NGE {
	namespace Scripting {

		class ScriptManager {
		  public:
			virtual bool Init() = 0;
			virtual bool ExecuteFile(const std::string& resource) = 0;
			virtual bool ExecuteString(const std::string& str) = 0;
		};
	}
}

#endif /* ISCRIPTMANAGER_HPP */

