/* 
 * File:   TouchEvent.hpp
 * Author: tku
 *
 * Created on 6 lipiec 2015, 00:40
 */

#ifndef TOUCHEVENT_HPP
#define	TOUCHEVENT_HPP

#include <iostream>
#include "NGE/Events/EventsTypes.hpp"

namespace NGE {
    namespace Events {

        class TouchEvent {
          public:
            TouchEvent();
            TouchEvent(int id, TouchEvents type, int xPosition, int yPosition);
            TouchEvent& operator=(const TouchEvent& copy);

            int getId() const;
            void setId(int id);

            TouchEvents getType() const;
            void setType(TouchEvents type);

            int getXPosition() const;
            void setXPosition(int xPosition);

            int getYPosition() const;
            void setYPosition(int yPosition);

            int getNumTouches() const;
            void setNumTouches(int numTouches);

          private:
            int id;
            TouchEvents type;
            int xPosition, yPosition;
            int numTouches;
        };

        template <typename T> std::ostream &operator<<(std::ostream &out, const TouchEvent &event) {
            return out << "TouchEvent [id = " << event.getId()
                    << ", type = " << event.getType()
                    << ", xPosition = " << event.getXPosition()
                    << ", yPosition = " << event.getYPosition()
                    << ", numTouches = " << event.getNumTouches()
                    << "]";
        }
    }
}

#endif	/* TOUCHEVENT_HPP */

