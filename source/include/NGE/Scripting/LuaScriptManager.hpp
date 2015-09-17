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
			std::shared_ptr<sel::State> luaState;
			std::string lastError;

		  public:

			static LuaScriptManager& GetInstance() {
				static LuaScriptManager instance;
				return instance;
			}

			virtual bool Init();
			virtual bool ExecuteFile(const std::string& path);
			virtual bool ExecuteString(const std::string& str);

			std::string GetLastError();
			std::weak_ptr<sel::State> GetLuaState();

		  private:
			/**
			 * LuaScriptManager constructor is private, use GetInstance() method
			 * to get object.
			 */
			explicit LuaScriptManager();
			virtual ~LuaScriptManager();

			void SetError();
		};
	}
}

#endif	/* LUASCRIPTMANAGER_HPP */

