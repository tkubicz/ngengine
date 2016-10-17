/* 
 * File:   GUIText.hpp
 * Author: tku
 *
 * Created on 18 March 2013, 17:26
 */

#ifndef GUITEXT_HPP
#define	GUITEXT_HPP

#include <string>
#include "NGE/Math/Vector2.hpp"
#include "NGE/Math/Vector4.hpp"
#include <pugixml.hpp>
#include "NGE/Core/Core.hpp"

#ifdef NGE_USE_FREETYPE
#include "NGE/Media/Fonts/FreeTypeFont.hpp"
#endif

namespace NGE {
	namespace GUI {

		class GUIText {
		  public:
			GUIText(const std::string& text);
			GUIText(const char* text = NULL);
			GUIText(const GUIText& copy);

			virtual ~GUIText();

			GUIText& operator=(const GUIText& copy);
			GUIText& operator=(const std::string& text);
			GUIText& operator=(const char* text);

			virtual bool LoadXMLSettings(const pugi::xml_node& node);

			void ComputeDimensions();
			void PrintCenteredXY(int x, int y);
			void PrintCenteredX(int x, int y);
			void PrintCenteredY(int x, int y);
			void Print(int x, int y);

			const std::string& GetString();

			void SetString(const std::string& text);
			void SetString(const char* text);
			void Clear();

			void SetSize(int x, int y);
			void SetSize(const Math::vec2i& size);
			const Math::vec2i& GetSize();

			void SetColor(float r, float g, float b, float a = 1.0);
			void SetColor(const Math::vec3i& color);
			void SetColor(const Math::vec4f& color);
			const Math::vec4f& GetColor();

			int GetWidth();
			int GetHeight();

			void SetHeightScale(float height);
			void SetWidthScale(float width);
			void SetScales(const Math::vec2f& scales);

			float GetHeightScale();
			float GetWidthScale();
			const Math::vec2f& GetScales();

			bool NeedUpdating();
			void ForceUpdate(bool update);

#ifdef NGE_USE_FREETYPE
			void SetFont(Media::Fonts::FreeTypeFont* font);
			Media::Fonts::FreeTypeFont* GetFont();
#endif

		  protected:
			bool update;
			std::string text;
			Math::vec2i position, size;
			Math::vec2f scales;
			Math::vec4f color;
#ifdef NGE_USE_FREETYPE
			Media::Fonts::FreeTypeFont* font;
#endif

			GLuint vertexBuffer;
			unsigned vertexSize;
		};
	}
}

#endif	/* GUITEXT_HPP */

