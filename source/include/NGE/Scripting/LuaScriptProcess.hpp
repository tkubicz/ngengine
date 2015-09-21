/* 
 * File:   LuaScriptProcess.hpp
 * Author: tku
 *
 * Created on 10 września 2015, 15:37
 */

#ifndef LUASCRIPTPROCESS_HPP
#define	LUASCRIPTPROCESS_HPP

#include "NGE/Core/Process.hpp"
#include "NGE/ThirdPart/selene.h"

namespace NGE {
	namespace Scripting {

		class LuaScriptProcess : public NGE::Core::Process {
		  private:
			lua_Number frequency, time;
			std::function<void() > scriptInitFunction;
			std::function<void(lua_Number) > scriptUpdateFunction;
			std::function<void() > scriptSuccessFunction, scriptFailFunction;
			std::function<void() > scriptSelf;

		  public:
			LuaScriptProcess();

			bool BuildProcessFromScript(const std::string& name);
			
			static void RegisterScriptClass();
			static void CreateFromScript(const std::string className, const std::string variableName);

		  protected:
			virtual void OnInit() override;
			virtual void OnUpdate(unsigned int deltaTime) override;
			virtual void OnSuccess() override;
			virtual void OnFail() override;
			virtual void OnAbort() override;

		  public:

			void ScriptAttachChild(NGE::Core::Process* child) { }

			// Methods belowe are here because selene (at least for now) could not
			// create classes without implementation in current class.

			virtual void Succeed() override;
			virtual void Fail() override;
			virtual void Pause() override;
			virtual void UnPause() override;
			virtual bool IsAlive() const override;
			virtual bool IsDead() const override;
			virtual bool IsRemoved() const override;
			virtual bool IsPaused() const override;

			unsigned long int GetFrequency() const;
			unsigned long int GetTime() const;
		};
	}
}

#endif	/* LUASCRIPTPROCESS_HPP */

