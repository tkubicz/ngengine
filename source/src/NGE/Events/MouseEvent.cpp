#include "NGE/Events/MouseEvent.hpp"
using namespace NGE::Events;

constexpr const EventType MouseEvent::eventType;

MouseEvent::MouseEvent(float timeStamp) : BaseEventData(timeStamp) {
    this->x = this->y = this->yInverse = 0;
    this->action = NOACTION;
    this->scrolling = NOSCROLLING;
    this->buttonId = NOBUTTON;
    moved = false;
}

MouseEvent::MouseEvent(int id, int action, int xPosition, int yPosition, int yInverse, int scrolling, float timeStamp) : BaseEventData(timeStamp) {
    buttonId = id;
    this->action = action;
    x = xPosition;
    y = yPosition;
    this->yInverse = yInverse;
    this->scrolling = scrolling;
}

MouseEvent& MouseEvent::operator=(const MouseEvent& copy) {
    if (this != &copy) {
        buttonId = copy.buttonId;
        action = copy.action;
        x = copy.x;
        y = copy.y;
        yInverse = copy.yInverse;
        scrolling = copy.scrolling;
    }
    return *this;
}

int MouseEvent::GetScrolling() {
    return scrolling;
}

int MouseEvent::GetButtonId() {
    return buttonId;
}

int MouseEvent::GetAction() {
    return action;
}

bool MouseEvent::GetMoved() {
    return moved;
}

void MouseEvent::SetMoved(bool moved) {
    this->moved = moved;
}

NGE::Math::vec2i MouseEvent::GetPosition() {
    return Math::vec2i(x, y);
}

int MouseEvent::GetX() {
    return x;
}

int MouseEvent::GetY() {
    return y;
}

int MouseEvent::GetYInverse() {
    return yInverse;
}
