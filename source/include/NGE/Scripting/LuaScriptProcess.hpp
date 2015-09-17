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
			sel::Selector scriptInitFunction, scriptUpdateFunction;
			sel::Selector scriptSuccessFunction, scriptFailFunction;
			sel::Selector scriptSelf;

		  public:
			LuaScriptProcess();

			static void RegisterScriptClass();

		  protected:
			virtual void OnInit();
			virtual void OnUpdate(unsigned int deltaTime);
			virtual void OnSuccess();
			virtual void OnFail();
			virtual void OnAbort();

		  private:

		};
	}
}

#endif	/* LUASCRIPTPROCESS_HPP */

