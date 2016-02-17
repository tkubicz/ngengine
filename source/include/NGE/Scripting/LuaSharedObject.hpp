/* 
 * File:   LuaSharedObject.hpp
 * Author: tku
 *
 * Created on 17 lutego 2016, 17:48
 */

#ifndef LUASHAREDOBJECT_HPP
#define LUASHAREDOBJECT_HPP

namespace NGE {
	namespace Scripting {

		class LuaSharedObject {
		  public:

			template <class T> static std::shared_ptr<T> CreateSharedObject() {
				return std::make_shared<T>();
			}
		};
	}
}

#endif /* LUASHAREDOBJECT_HPP */

