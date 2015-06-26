/* 
 * File:   MouseEvent.hpp
 * Author: tku
 *
 * Created on 6 styczeń 2013, 09:01
 */

#ifndef MOUSEEVENT_HPP
#define	MOUSEEVENT_HPP

#include "NGE/Events/EventsTypes.hpp"
#include "NGE/Math/Vector2.hpp"

namespace NGE {
	namespace Events {

		class MouseEvent {
		  public:
			MouseEvent();
			MouseEvent(int id, int action, int xPosition, int yPosition, int yInverse, int scrolling);
			MouseEvent& operator=(const MouseEvent& copy);

			int GetScrolling();
			int GetButtonId();
			int GetAction();

			bool GetMoved();
			void SetMoved(bool moved);

			Math::vec2i GetPosition();
			int GetX();
			int GetY();
			int GetYInverse();

		  private:
			int scrolling, buttonId, action,
			x, y, yInverse;
			bool moved;
		};
	}
}


#endif	/* MOUSEEVENT_HPP */

