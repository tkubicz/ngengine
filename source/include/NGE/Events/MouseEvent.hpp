/* 
 * File:   MouseEvent.hpp
 * Author: tku
 *
 * Created on 6 styczeń 2013, 09:01
 */

#ifndef MOUSEEVENT_HPP
#define MOUSEEVENT_HPP

#include "NGE/Events/BaseEventData.hpp"
#include "NGE/Events/EventsTypes.hpp"
#include "NGE/Math/Vector2.hpp"

namespace NGE {
    namespace Events {

        class MouseEvent : public BaseEventData {
          private:

            int scrolling, buttonId, action,
            x, y, yInverse;
            bool moved;

          public:
            static constexpr const EventType eventType = 0x9bad1a15;

            MouseEvent(float timeStamp = 0.0f);
            MouseEvent(int id, int action, int xPosition, int yPosition, int yInverse, int scrolling, float timeStamp = 0.0f);
            MouseEvent& operator=(const MouseEvent& copy);

            const EventType& GetEventType() const override {
                return eventType;
            }

            const std::string GetName() const override {
                return "MouseEvent";
            }

            IEventDataPtr Copy() const override {
                return IEventDataPtr(new MouseEvent(buttonId, action, x, y, yInverse, scrolling));
            }

            int GetScrolling();
            int GetButtonId();
            int GetAction();

            bool GetMoved();
            void SetMoved(bool moved);

            Math::vec2i GetPosition();
            int GetX();
            int GetY();
            int GetYInverse();
        };
    }
}


#endif /* MOUSEEVENT_HPP */

