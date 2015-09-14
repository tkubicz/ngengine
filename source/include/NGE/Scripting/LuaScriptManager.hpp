/* 
 * File:   LuaScriptManager.hpp
 * Author: tku
 *
 * Created on 14 września 2015, 16:09
 */

#ifndef LUASCRIPTMANAGER_HPP
#define	LUASCRIPTMANAGER_HPP

#include "NGE/ThirdPart/selene.h"
#include "NGE/Scripting/IScriptManager.hpp"

namespace NGE {
	namespace Scripting {

		class LuaScriptManager : public IScriptManager {
		  private:
			sel::State* luaState;
			std::string lastError;

		  public:

			static LuaScriptManager& GetInstance() {
				static LuaScriptManager instance;
				return instance;
			}

			virtual bool Init();
			virtual void ExecuteFile(const std::string& resource);
			virtual void ExecuteString(const std::string& str);


		};
	}
}

#endif	/* LUASCRIPTMANAGER_HPP */

