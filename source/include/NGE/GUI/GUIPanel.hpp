/* 
 * File:   GUIPanel.hpp
 * Author: tku
 *
 * Created on 6 styczeń 2013, 00:19
 */

#ifndef GUIPANEL_HPP
#define	GUIPANEL_HPP

#include "NGE/GUI/GUIRectangle.hpp"
#include "NGE/GUI/GUIClippedRectangle.hpp"
#include "NGE/Geometry/DistanceObject.hpp"
#include <pugixml.hpp>

namespace NGE {
	namespace GUI {
		typedef Geometry::DistanceObject<GUIRectangle*> ZWidget;
		typedef std::vector<GUIRectangle*> Widgets;
		typedef std::vector<ZWidget> ZWidgets;

		class GUIPanel : public GUIRectangle, public GUIClippedRectangle {
		  protected:
			Widgets elements;
			Math::vec2i interval;
			int layout;

		  private:
			void PackFreeLayout();
			void PackYAxisLayout();
			void PackXAxisLayout();
			void CorrectPosition();

		  public:
			GUIPanel(const char* callback = NULL);
			virtual ~GUIPanel();
			GUIRectangle* GetWidgetByCallbackString(const char* name);

			void SetInterval(int width, int height);
			void SetInterval(const Math::vec2i& dimensions);
			const Math::vec2i& GetInterval();

			void SetLayout(int layout);
			int GetLayout();

			int GetTreeHeight();

			virtual bool LoadXMLSettings(const pugi::xml_node& node);

			virtual void Notify(GUIRectangle* element);
			virtual void CheckMouseEvents(Events::MouseEvent& event);
			virtual void CheckKeyboardEvents(Events::KeyboardEvent& event);

			virtual void Update(float dt);
			virtual void Render();

			virtual void CollectZWidgets(ZWidgets& container);
			virtual void ForceUpdate(bool);
			virtual bool AddWidget(GUIRectangle* widget);
			virtual void Pack();
			virtual void Clear();

			int GetWidgetCountByType(int type);
			Widgets& GetWidgets();
		};
	}
}

#endif	/* GUIPANEL_HPP */

