#include "NGE/Events/KeyboardEvent.hpp"
using namespace NGE::Events;

KeyboardEvent::KeyboardEvent()
{
    keyId = KEY_EMPTY;
    action = NOACTION;
}

KeyboardEvent::KeyboardEvent(int id, int action, int modifier)
{
    keyId = id;
    this->action = action;
    this->modifier = modifier;
}

KeyboardEvent& KeyboardEvent::operator =(const KeyboardEvent& copy)
{
    if (this != &copy)
    {
        keyId = copy.keyId;
        action = copy.action;
        modifier = copy.modifier;
    }
    return *this;
}

int KeyboardEvent::GetAction()
{
    return action;
}

int KeyboardEvent::GetKeyId()
{
    return keyId;
}

int KeyboardEvent::GetModifier()
{
    return modifier;
}

bool KeyboardEvent::CheckModifier(int value)
{
    return (modifier & value) == value;
}