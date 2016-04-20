/* 
 * File:   Delegate.hpp
 * Author: tku
 *
 * Created on 7 September 2015, 11:32
 */

#ifndef DELEGATE_HPP
#define DELEGATE_HPP

#include <iostream>
#include <utility>

namespace NGE {
	namespace Core {

		template <typename Obj, typename Result, typename ...Args>
		struct Delegate {
			typedef Result(Obj::*func_type)(Args...);

			Obj& x;
			func_type f;

			Result operator()(Args... args) {
				return (x.*f)(args...);
			}

			template <typename CompareObj, typename CompareResult, typename ...CompareArgs>
			bool operator==(const Delegate<CompareObj, CompareResult, CompareArgs...>& other) const {
				return (void*) &x == (void*) &other.x && typeid(f) == typeid(other.f);
			}

			bool operator==(const Delegate<Obj, Result, Args...>& other) {
				return (void*) &x == (void*) &other.x && f == other.f;
			}
		};

		template <typename Obj, typename Result, typename ...Args>
		auto make_delegate(Obj &x, Result(Obj::*fun)(Args...)) -> Delegate<Obj, Result, Args...> {
			return Delegate<Obj, Result, Args...>{x, fun};
		}
	}
}

#endif /* DELEGATE_HPP */

