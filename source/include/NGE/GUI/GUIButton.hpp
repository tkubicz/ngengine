/* 
 * File:   GUIButton.h
 * Author: tku
 *
 * Created on 23 marzec 2013, 02:06
 */

#ifndef GUIBUTTON_HPP
#define	GUIBUTTON_HPP

#include "NGE/GUI/GUIClippedRectangle.hpp"
#include "NGE/GUI/GUIRectangle.hpp"
#include "NGE/GUI/GUIAlphaElement.hpp"

namespace NGE {
	namespace GUI {

		class GUIButton : public GUIAlphaElement, public GUIClippedRectangle {
		  public:
			GUIButton(const char* callback = NULL);

			virtual void Update(float dt);
			virtual void Render();

			virtual bool LoadXMLSettings(const pugi::xml_node& node);
			virtual const void ComputeWindowBounds();

			void EnableBounce(bool bounce);
			bool BounceEnabled();

		  private:
			bool bounce;
		};
	}
}

#endif	/* GUIBUTTON_HPP */

