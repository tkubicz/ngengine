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
#include "NGE/Core/Singleton.hpp"
#include "NGE/Scripting/ScriptManager.hpp"

namespace NGE {
    namespace Scripting {

        /**
         * Class that manages state of the Lua in the engine. This class is a
         * singleton. This class requires initialisation before first usage, so
         * do not forget to invoke Init() method.
         */
        class LuaScriptManager : public NGE::Core::Singleton<LuaScriptManager>, public ScriptManager {
            friend class NGE::Core::Singleton<LuaScriptManager>;

          private:
            std::shared_ptr<kaguya::State> luaState;
            std::string lastError;

          public:

            /**
             * Initialise Lua state. This method should be invoked first.
             * @return True if the state was initialised successfully, otherwise false.
             */
            virtual bool Initialise() override;

            virtual bool ExecuteFile(const std::string& path) override;
            virtual bool ExecuteString(const std::string& str) override;

            void SetLastError(const char* error);
            std::string GetLastError() const;

            std::weak_ptr<kaguya::State> GetLuaState();

          private:
            /**
             * LuaScriptManager constructor is private, use LuaScriptManager::GetInstance() method
             * to get object.
             */
            explicit LuaScriptManager();

            ~LuaScriptManager() { }

            static void HandleError(int val, const char* msg);
        };
    }
}

#endif /* LUASCRIPTMANAGER_HPP */

