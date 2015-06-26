/* 
 * File:   FreeTypeFont.hpp
 * Author: tku
 *
 * Created on 21 luty 2013, 22:39
 */

#ifndef FREETYPEFONT_HPP
#define	FREETYPEFONT_HPP

#ifdef NGE_USE_FREETYPE

#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "NGE/Math/Vector2.hpp"
#include "NGE/Math/Vector4.hpp"

namespace NGE {
	namespace Media {
		namespace Fonts {

			class FreeTypeFont {
			  public:
				FreeTypeFont(int fontSize = 16, const std::string& fontName = "", Media::Shaders::GLSLProgram* shader = NULL);
				virtual ~FreeTypeFont();

				bool Initialize();
				bool LoadXMLSettings(const pugi::xml_node& node, const std::string& path);

				void GenerateVertices(GLuint& buffer, unsigned& size, const std::string& str, Math::vec2f position, const Math::vec2f& scale);

				void PrintString(const std::string& str, Math::vec2f position, Math::vec2f scale, Math::vec4f color = Math::vec4f(0.0, 0.0, 0.0, 1.0f));
				void PrintString(const GLuint& vertexBuffer, const unsigned& size, const Math::vec4f& color);

				Math::vec2i GetStringDimensions(const std::string& str, const Math::vec2f& scale = Math::vec2f(1, 1));
				int GetStringWidth(const std::string& str);
				int GetStringHeight(const std::string& str);

				void SetShader(Media::Shaders::GLSLProgram* shader);
				Media::Shaders::GLSLProgram* GetShader();

				void SetFontName(const std::string& fontName);
				const std::string& GetFontName();

				void SetFontSize(int fontSize);
				int GetFontSize();

				int GetAtlasHeight();
				const std::map<char, std::pair<int, int> >& GetGlyphDimensions();
				const std::map<char, std::pair<int, int> >& GetGlyphAdvances();

			  private:
				GLuint vertexBuffer, atlasTex;

				static const int maxWidth = 1024;
				int fontSize, atlasWidth, atlasHeight;
				std::string fontName;

				Media::Shaders::GLSLProgram* shader;

				std::map<char, std::pair<int, int> > glyphDimensions;
				std::map<char, std::pair<int, int> > glyphPositions;
				std::map<char, std::pair<int, int> > glyphAdvances;
				std::map<char, std::pair<float, float> > glyphTextureOffset;

				bool GenerateAtlasTexture(FT_Face& fontInfo);
				bool GenerateSeperateTextures(FT_Face& fontInfo);
				bool GenerateCharacterTexture(unsigned char ch, FT_Face fontInfo);
			};
		}
	}
}

#endif

#endif	/* FREETYPEFONT_HPP */

