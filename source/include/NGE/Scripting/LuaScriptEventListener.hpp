/* 
 * File:   LuaScriptEventListener.hpp
 * Author: tku
 *
 * Created on 17 września 2015, 15:41
 */

#ifndef LUASCRIPTEVENTLISTENER_HPP
#define	LUASCRIPTEVENTLISTENER_HPP

namespace NGE
{
	namespace Scripting
	{
		class LuaScriptEventListener {
		  public:
			LuaScriptEventListener(sel::Selector explicitHandlerFunction);
			virtual ~LuaScriptEventListener();
			
			virtual std::string GetName() const {
				return "Script Listener";
			}
			
		};
	}
}

#endif	/* LUASCRIPTEVENTLISTENER_HPP */

