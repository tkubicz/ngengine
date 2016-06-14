/* 
 * File:   GUISeperator.hpp
 * Author: tku
 *
 * Created on 3 lipiec 2012, 22:46
 */

#ifndef GUISEPARATOR_HPP
#define GUISEPARATOR_HPP

#include "NGE/GUI/GUIRectangle.hpp"
#include "NGE/GUI/GUITypes.hpp"
#include <pugixml.hpp>

namespace NGE {
	namespace GUI {

		class GUISeparator : public GUIRectangle {
		  public:
			GUISeparator(Orientation orientation = HORIZONTAL);

			virtual ~GUISeparator() { }

			virtual bool LoadXMLSettings(const pugi::xml_node& node);

			virtual void Update(float dt) override;
			virtual void Render() override;

			virtual const Math::vec4i& GetWindowBounds() override;
			virtual const void ComputeWindowBounds() override;

			void SetOrientation(Orientation orientation);
			void SetOrientation(const std::string& orientation);
			Orientation GetOrientation();

			void SetColor(float r, float g, float b);
			const Math::vec3f& GetColor();

			void SetRatio(float ratio);
			float GetRatio();

		  private:
			Orientation orientation;
			float ratio;
			Math::vec3f color;

			Media::Shaders::GLSLProgram* shader;
			GLuint vertexBuffer;
			Math::vec4f vertices[2];
		};
	}
}

#endif /* GUISEPARATOR_HPP */

