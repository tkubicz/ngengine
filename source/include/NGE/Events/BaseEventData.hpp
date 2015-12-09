/* 
 * File:   BaseEventData.hpp
 * Author: tku
 *
 * Created on 7 September 2015, 11:13
 */

#ifndef BASEEVENTDATA_HPP
#define	BASEEVENTDATA_HPP

#include "IEventData.hpp"


namespace NGE {
	namespace Events {

		class BaseEventData : public IEventData {
		  private:
			const float timeStamp;

		  public:

			explicit BaseEventData(const float timeStamp = 0.0f) : timeStamp(timeStamp) { }

			/**
			 * Get the type of the event.
			 * @return Type of the event.
			 */
			virtual const EventType& GetEventType() const = 0;

			float GetTimeStamp() const {
				return timeStamp;
			}

			/**
			 * Serialise event for network input/output.
			 * @param out
			 */
			virtual void Serialize(std::ostream& out) const { }

			/**
			 * Deserialise event for network input/output.
			 * @param in
			 */
			virtual void Deserialize(std::istream& in) { }

		};
	}
}

#endif	/* BASEEVENTDATA_HPP */

