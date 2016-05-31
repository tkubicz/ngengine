/* 
 * File:   GenericObjectFactory.hpp
 * Author: tku
 *
 * Created on 12 May 2016, 17:32
 */

#ifndef GENERICOBJECTFACTORY_HPP
#define GENERICOBJECTFACTORY_HPP

namespace NGE {
	namespace Core {

		template <typename BaseClass, typename SubClass> BaseClass* GenericObjectCreationFunction() {
			return new SubClass;
		}

		template <typename BaseClass, typename IdType> class GenericObjectFactory {
			typedef BaseClass* (*ObjectCreationFunction)();
			std::map<IdType, ObjectCreationFunction> creationFunctions;

		  public:

			template <class SubClass> bool Register(IdType id) {
				auto findIt = creationFunctions.find(id);
				if (findIt == creationFunctions.end()) {
					creationFunctions[id] = &GenericObjectCreationFunction<BaseClass, SubClass>;
					return true;
				}
				return false;
			}

			BaseClass* Create(IdType id) {
				auto findIt = creationFunctions.find(id);
				if (findIt != creationFunctions.end()) {
					ObjectCreationFunction func = findIt->second;
					return func();
				}
				return nullptr;
			}
		};
	}
}

#endif /* GENERICOBJECTFACTORY_HPP */

