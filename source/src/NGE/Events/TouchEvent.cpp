#include "NGE/Events/TouchEvent.hpp"
using namespace NGE::Events;

TouchEvent::TouchEvent() : id(0), type(TOUCHEVENT_NOACTION), xPosition(0), yPosition(0) { }

TouchEvent::TouchEvent(int id, TouchEvents type, int xPosition, int yPosition) : id(id), type(type), xPosition(xPosition), yPosition(yPosition) { }

TouchEvent& TouchEvent::operator=(const TouchEvent& copy) {
    if (this != &copy) {
        id = copy.id;
        type = copy.type;
        xPosition = copy.xPosition;
        yPosition = copy.yPosition;
    }
    return *this;
}

int TouchEvent::getId() const {
    return id;
}

void TouchEvent::setId(int id) {
    this->id = id;
}

TouchEvents TouchEvent::getType() const {
    return type;
}

void TouchEvent::setType(TouchEvents type) {
    this->type = type;
}

int TouchEvent::getXPosition() const {
    return xPosition;
}

void TouchEvent::setXPosition(int xPosition) {
    this->xPosition = xPosition;
}

int TouchEvent::getYPosition() const {
    return yPosition;
}

void TouchEvent::setYPosition(int yPosition) {
    this->yPosition = yPosition;
}

int TouchEvent::getNumTouches() const {
    return numTouches;
}

void TouchEvent::setNumTouches(int numTouches) {
    this->numTouches = numTouches;
}







