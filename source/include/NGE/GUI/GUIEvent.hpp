/* 
 * File:   GUIEvent.hpp
 * Author: tku
 *
 * Created on 23 styczeń 2013, 16:03
 */

#ifndef GUIEVENT_HPP
#define	GUIEVENT_HPP

#include <string>

namespace NGE {
	namespace GUI {
		class GUIRectangle;

		class GUIEvent {
		  public:
			GUIEvent(GUIRectangle* element);
			GUIEvent(const GUIEvent& copy);
			GUIEvent& operator=(const GUIEvent& copy);

			GUIRectangle* GetEventSource();
			const std::string& GetCallbackString();

		  private:
			GUIRectangle* eventSource;
			std::string callbackString;
		};
	}
}

#endif	/* GUIEVENT_HPP */

