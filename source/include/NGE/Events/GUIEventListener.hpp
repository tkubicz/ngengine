/* 
 * File:   GUIEventListener.h
 * Author: tku
 *
 * Created on 23 styczeń 2013, 16:08
 */

#ifndef GUIEVENTLISTENER_HPP
#define	GUIEVENTLISTENER_HPP

#include "NGE/GUI/GUIEvent.hpp"

namespace NGE {
	namespace Events {

		class GUIEventListener {
		  public:
			virtual void ActionPerformed(GUI::GUIEvent& event) = 0;
		};
	}
}

#endif	/* GUIEVENTLISTENER_HPP */

