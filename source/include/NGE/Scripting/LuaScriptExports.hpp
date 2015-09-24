/* 
 * File:   LuaScriptExports.hpp
 * Author: tku
 *
 * Created on 24 września 2015, 15:13
 */

#ifndef LUASCRIPTEXPORTS_HPP
#define	LUASCRIPTEXPORTS_HPP

namespace NGE {
	namespace Scripting {

		class LuaScriptExports {
		  public:
			/**
			 * This method registers all the global functions with the script system. It should
			 * be called when application is beeing initialized.
			 */
			static void Register();
		};
	}
}

#endif	/* LUASCRIPTEXPORTS_HPP */

