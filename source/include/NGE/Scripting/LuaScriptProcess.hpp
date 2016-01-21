/* 
 * File:   LuaScriptProcess.hpp
 * Author: tku
 *
 * Created on 21 January 2016, 11:06
 */

#ifndef LUASCRIPTPROCESS_HPP
#define LUASCRIPTPROCESS_HPP

#include <kaguya/kaguya.hpp>
#include "NGE/Core/Process.hpp"

namespace NGE {
	namespace Scripting {

		class LuaScriptProcess : public NGE::Core::Process {
		  private:
			unsigned int frequency, time;
			kaguya::LuaFunction initFunction, updateFunction, successFunction, failFunction, abortFunction;

		  public:

			explicit LuaScriptProcess() {}
			static void RegisterScriptClass();
			
			void OnInitWrapper() {
				OnInit();
			}
			
			void OnUpdateWrapper(unsigned int v) {
				OnUpdate(v);
			}

		  protected:
			virtual void OnInit() override;
			virtual void OnUpdate(unsigned int deltaMs) override;
			virtual void OnSuccess() override;
			virtual void OnFail() override;
			virtual void OnAbort() override;

		  private:

			bool BuildDataFromScript(kaguya::LuaRef self);
			void ScriptAttachChild(kaguya::LuaRef child);
		};
	}
}

#endif /* LUASCRIPTPROCESS_HPP */

