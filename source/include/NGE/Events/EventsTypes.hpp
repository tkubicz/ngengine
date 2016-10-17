/* 
 * File:   EventsTypes.hpp
 * Author: tku
 *
 * Created on 3 April 2013, 02:21
 */

#ifndef EVENTSTYPES_HPP
#define	EVENTSTYPES_HPP

namespace NGE {
    namespace Events {

        enum InputAction {
            NOACTION, PRESSED, RELEASED, REPEAT
        };

        enum KeyboardCharacter {
            KEY_EMPTY = 0,
            KEY_SPACE = 32,
            KEY_ZERO = 48, KEY_ONE, KEY_TWO, KEY_THREE, KEY_FOUR, KEY_FIVE, KEY_SIX, KEY_SEVEN, KEY_EIGHT, KEY_NINE, KEY_TEN,
            KEY_A = 65, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P,
            KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
            KEY_ESCAPE = 256, KEY_ENTER, KEY_TAB,
            KEY_RIGHT = 262, KEY_LEFT, KEY_DOWN, KEY_UP,
            KEY_F1 = 290, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
            KEY_NUM0 = 320, KEY_NUM1, KEY_NUM2, KEY_NUM3, KEY_NUM4, KEY_NUM5, KEY_NUM6, KEY_NUM7, KEY_NUM8, KEY_NUM9,
            KEY_NUM_SLASH = 331, KEY_NUM_ASTERIKS, KEY_NUM_MINUS, KEY_NUM_PLUS, KEY_RIGHT_ENTER,
            KEY_LSHIFT = 340, KEY_LCTRL, KEY_LALT, KEY_WIN, KEY_RSHIFT, KEY_RCTRL, KEY_RALT
        };

        enum KeyboardModifier {
            MODIFIER_NONE = 1,
            MODIFIER_LSHIFT = 2,
            MODIFIER_RSHIFT = 4,
            MODIFIER_LCTRL = 8,
            MODIFIER_RCTRL = 16,
            MODIFIER_LALT = 32,
            MODIFIER_RALT = 64
        };

        enum MouseEvents {
            NOBUTTON, BUTTON1, BUTTON2, BUTTON3,
            NOSCROLLING, SCROLLING_UP, SCROLLING_DOWN
        };

        enum TouchEvents {
            TOUCHEVENT_NOACTION,
            TOUCHEVENT_BEGAN,
            TOUCHEVENT_ENDED,
            TOUCHEVENT_MOVED,
            TOUCHEVENT_CANCELLED
        };
    }
}

#endif	/* EVENTSTYPES_HPP */

