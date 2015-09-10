/* 
 * File:   Delegate.hpp
 * Author: tku
 *
 * Created on 7 września 2015, 11:32
 */

#ifndef DELEGATE_HPP
#define	DELEGATE_HPP

namespace NGE {
	namespace Core {

		template <typename Obj, typename Result, typename ...Args>
		struct Delegate {
			Obj x;
			Result(Obj::*f)(Args...);

			template <typename ...Ts>
			Result operator()(Ts&&... args) {
				return (x.*f)(std::forward<Ts>(args)...);
			}
		};

		template <typename Obj, typename Result, typename ...Args>
		auto make_delegate(const Obj &x, Result(Obj::*fun)(Args...)) -> Delegate<Obj, Result, Args...> {
			Delegate<Obj, Result, Args...> result{x, fun};
			return result;
		}
	}
}

#endif	/* DELEGATE_HPP */

