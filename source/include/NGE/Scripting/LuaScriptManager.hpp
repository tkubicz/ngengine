/* 
 * File:   LuaScriptManager.hpp
 * Author: tku
 *
 * Created on 14 September 2015, 16:09
 */

#ifndef LUASCRIPTMANAGER_HPP
#define LUASCRIPTMANAGER_HPP

#include <memory>
#include <kaguya/kaguya.hpp>
#include "NGE/Scripting/ScriptManager.hpp"

namespace NGE {
	namespace Scripting {

		class LuaScriptManager : public ScriptManager {
		  private:
			std::shared_ptr<kaguya::State> luaState;
			std::string lastError;

		  public:

			~LuaScriptManager() { }

			static LuaScriptManager& GetInstance() {
				static LuaScriptManager instance;
				return instance;
			}

			virtual bool Init();
			virtual bool ExecuteFile(const std::string& path);
			virtual bool ExecuteString(const std::string& str);

			void SetLastError(const char* error);
			std::string GetLastError() const;
			
			std::weak_ptr<kaguya::State> GetLuaState();

		  private:
			/**
			 * LuaScriptManager constructor is private, use GetInstance() method
			 * to get object.
			 */
			explicit LuaScriptManager();
			
			static void HandleError(int val, const char* msg);
		};
	}
}

#endif /* LUASCRIPTMANAGER_HPP */

