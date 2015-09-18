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
			unsigned long int frequency, time;
			std::function<void() > scriptInitFunction, scriptUpdateFunction;
			std::function<void() > scriptSuccessFunction, scriptFailFunction;
			std::function<void() > scriptSelf;

		  public:
			LuaScriptProcess();

			static void RegisterScriptClass();

		  protected:
			virtual void OnInit() override;
			virtual void OnUpdate(unsigned int deltaTime) override;
			virtual void OnSuccess() override;
			virtual void OnFail() override;
			virtual void OnAbort() override;

		  public:
			
			void ScriptAttachChild(NGE::Core::Process* child) {}

			// Methods belowe are here because selene (at least for now) could not
			// create classes without implementation in current class.

			virtual void Succeed() override {
				NGE::Core::Process::Succeed();
			}

			virtual void Fail() override {
				NGE::Core::Process::Fail();
			}

			virtual void Pause() override {
				NGE::Core::Process::Pause();
			}

			virtual void UnPause() override {
				NGE::Core::Process::UnPause();
			}

			virtual bool IsAlive() const override {
				return NGE::Core::Process::IsAlive();
			}

			virtual bool IsDead() const override {
				return NGE::Core::Process::IsDead();
			}

			virtual bool IsRemoved() const override {
				return NGE::Core::Process::IsRemoved();
			}

			virtual bool IsPaused() const override {
				return NGE::Core::Process::IsPaused();
			}
		};
	}
}

#endif	/* LUASCRIPTPROCESS_HPP */

