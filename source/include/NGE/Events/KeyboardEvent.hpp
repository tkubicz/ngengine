/* 
 * File:   KeyboardEvent.hpp
 * Author: tku
 *
 * Created on 2 kwiecień 2013, 04:09
 */

#ifndef KEYBOARDEVENT_HPP
#define	KEYBOARDEVENT_HPP

#include "EventsTypes.hpp"

namespace NGE {
	namespace Events {

		class KeyboardEvent {
		  public:
			KeyboardEvent();
			KeyboardEvent(int id, int action, int modifier);
			KeyboardEvent& operator=(const KeyboardEvent& copy);

			int GetKeyId();
			int GetAction();
			int GetModifier();
			bool CheckModifier(int value);

		  private:
			int keyId, action, modifier;
		};
	}
}

#endif	/* KEYBOARDEVENT_HPP */

