/* 
 * File:   Delegate.hpp
 * Author: tku
 *
 * Created on 7 September 2015, 11:32
 */

#ifndef DELEGATE_HPP
#define DELEGATE_HPP

#include <utility>

namespace NGE {
	namespace Core {

		template <typename Obj, typename Result, typename ...Args>
		struct Delegate {
			Obj& x;
			Result(Obj::*f)(Args...);

			Result operator()(Args... args) {
				return (x.*f)(args...);
			}

			template <typename CompareObj, typename CompareResult, typename ...CompareArgs>
			bool operator==(const Delegate<CompareObj, CompareResult, CompareArgs...>& d1) {
				if (typeid (Obj) != typeid (CompareObj) || typeid (Result) != typeid (CompareResult)) {
					return false;
				}
				return (void*) &d1.x == (void*) &x && (void*) d1.f == (void*) f;
			}
		};

		template <typename Obj, typename Result, typename ...Args>
		auto make_delegate(Obj &x, Result(Obj::*fun)(Args...)) -> Delegate<Obj, Result, Args...> {
			return Delegate<Obj, Result, Args...>{x, fun};
		}
	}
}

#endif /* DELEGATE_HPP */

