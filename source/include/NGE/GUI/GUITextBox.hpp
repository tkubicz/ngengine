/* 
 * File:   GUITextBox.hpp
 * Author: tku
 *
 * Created on 3 kwiecień 2013, 21:33
 */

#ifndef GUITEXTBOX_HPP
#define	GUITEXTBOX_HPP

#include "NGE/GUI/GUIAlphaElement.hpp"
#include "NGE/Events/MouseEvent.hpp"
#include "NGE/Events/KeyboardEvent.hpp"
#include <pugixml.hpp>
#include "NGE/Media/Fonts/FreeTypeFont.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"

namespace NGE {
	namespace GUI {

		class GUITextBox : public GUIAlphaElement {
		  public:
			GUITextBox(const char* callbackString = NULL, const char* fieldText = NULL);
			virtual ~GUITextBox();

			virtual void Update(float dt);
			virtual void Render();

			virtual void CheckMouseEvents(Events::MouseEvent& event);
			virtual void CheckKeyboardEvents(Events::KeyboardEvent& event);

			virtual bool LoadXMLSettings(const pugi::xml_node& node);
			virtual const Math::vec4i& GetWindowBounds();

			void SetText(const std::string& text);
			const std::string& GetText();

			bool TextChanged();

			void SetBordersColor(const Math::vec3f& color);
			void SetBordersColor(float r, float g, float b);
			const Math::vec3f& GetBordersColor();

			void SetPadding(const Math::vec2i& padding);
			void SetPadding(int x, int y);
			const Math::vec2i& GetPadding();

#ifdef NGE_USE_FREETYPE
			void SetFont(Media::Fonts::FreeTypeFont* font);
			Media::Fonts::FreeTypeFont* GetFont();
#endif

		  protected:
			Math::vec2i padding;
			Math::vec3f bordersColor, textColor;
			float blinkerTimer;
			bool blinkerOn;
			int blinkerPosition, textStartIndex, textEndIndex;
			Events::KeyboardEvent keyEvent;

			GLuint vertexBuffer;
			Media::Shaders::GLSLProgram* shader;

		  private:
			void SetupText(int type, char* info = NULL);
			void SetupBlinker(int mouseX);
		};
	}
}

#endif	/* GUITEXTBOX_H */

