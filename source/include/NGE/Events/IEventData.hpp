/* 
 * File:   IEventData.hpp
 * Author: tku
 *
 * Created on 7 September 2015, 11:07
 */

#ifndef IEVENTDATA_HPP
#define IEVENTDATA_HPP

#include <memory>

namespace NGE {
	namespace Events {

		class IEventData;

		typedef unsigned long EventType;
		typedef std::shared_ptr<IEventData> IEventDataPtr;

		/**
		 * Base type for event object hierarchy, may be used itself for simplest 
		 * event notifications such as those that do not carry additional payload 
		 * data. If any event needs to propagate with payload data it must be defined
		 * separately.
		 */
		class IEventData {
		  public:

			virtual ~IEventData() { }
			virtual const EventType& GetEventType() const = 0;
			virtual float GetTimeStamp() const = 0;
			virtual void Serialize(std::ostream& out) const = 0;
			virtual void Deserialize(std::istream& in) = 0;
			virtual IEventDataPtr Copy() const = 0;
			virtual const std::string GetName() const = 0;
		};
	}
}

#endif /* IEVENTDATA_HPP */

