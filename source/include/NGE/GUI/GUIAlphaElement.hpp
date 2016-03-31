/* 
 * File:   GUIAlphaElement.hpp
 * Author: tku
 *
 * Created on 19 luty 2013, 10:39
 */

#ifndef GUIALPHAELEMENT_HPP
#define GUIALPHAELEMENT_HPP

#include "NGE/GUI/GUIRectangle.hpp"
#include "NGE/GUI/GUIText.hpp"

namespace NGE {
	namespace GUI {

		class GUIAlphaElement : public GUIRectangle {
		  public:
			GUIAlphaElement(const char* callback = NULL);

			virtual ~GUIAlphaElement() { }

			virtual void Update(float dt) = 0;
			virtual void Render() = 0;

			virtual bool LoadXMLSettings(const pugi::xml_node& node);

			void SetMinAlpha(float minAlpha);
			float GetMinAlpha();

			void SetAlphaFadeScale(float scale);
			float GetAlphaFadeScale();

			void SetAlpha(float alpha);
			float GetAlpha();

			void SetLabelString(const char* label);
			void SetLabelString(const std::string& label);
			const std::string& GetLabelString();

			void SetColor(const Math::vec3f& color);
			void SetColor(float r, float g, float b);
			const Math::vec4f& GetColor();

			GUIText* GetLabel();

		  protected:
			GUIText label;
			Math::vec4f color;
			float alphaFadeScale, minAlpha;

			void ModifyCurrentAlpha(float dt);
		};
	}
}

#endif /* GUIALPHAELEMENT_HPP */

