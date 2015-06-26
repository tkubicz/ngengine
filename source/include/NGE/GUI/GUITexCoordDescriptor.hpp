/* 
 * File:   GUITexCoordDescriptor.hpp
 * Author: tku
 *
 * Created on 11 listopad 2012, 21:51
 */

#ifndef GUITEXCOORDDESCRIPTOR_HPP
#define	GUITEXCOORDDESCRIPTOR_HPP

#include "NGE/GUI/GUITypes.hpp"
#include "NGE/Math/Vector4.hpp"

namespace NGE {
	namespace GUI {

		class GUITexCoordDescriptior {
		  private:
			void SetType(const std::string& type);

			Math::vec4f texCoords;
			WidgetTypes widgetType;

		  public:
			GUITexCoordDescriptior(const WidgetTypes type = UNKNOWN);
			GUITexCoordDescriptior(const GUITexCoordDescriptior& copy);
			GUITexCoordDescriptior& operator=(const GUITexCoordDescriptior& copy);

			static void SetTextureWidth(int width);
			static void SetTextureHeight(int height);

			static int GetTextureWidth();
			static int GetTextureHeight();

			// TODO: load settings from xml file
			//void LoadXMLSettings(XMLElement* element);

			void SetType(WidgetTypes type);
			WidgetTypes GetType();

			void SetTexCoords(float x, float y, float z, float w);
			void SetTexCoords(const Math::vec4f& texCoords);

			const Math::vec4f& GetTexCoords();
		};
	}
}

#endif	/* GUITEXCOORDDESCRIPTOR_HPP */

