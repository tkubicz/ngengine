/* 
 * File:   GUILabel.hpp
 * Author: tku
 *
 * Created on 31 marzec 2013, 03:08
 */

#ifndef GUILABEL_HPP
#define GUILABEL_HPP

#include "NGE/GUI/GUIAlphaElement.hpp"
#include <pugixml.hpp>

namespace NGE {
	namespace GUI {

		class GUILabel : public GUIAlphaElement {
		  public:
			GUILabel(const char* labelString = NULL, const char* callbackString = NULL);

			virtual ~GUILabel() { }

			virtual bool LoadXMLSettings(const pugi::xml_node& node);
			virtual const Math::vec4i& GetWindowBounds();

			virtual void Update(float dt);
			virtual void Render();
		};
	}
}

#endif /* GUILABEL_HPP */

