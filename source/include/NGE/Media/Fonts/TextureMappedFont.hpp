/* 
 * File:   TextureMappedFont.hpp
 * Author: tku
 *
 * Created on 20 luty 2013, 01:53
 */

#ifndef TEXTUREMAPPEDFONT_HPP
#define	TEXTUREMAPPEDFONT_HPP

#include <string>

#include "NGE/Parsers/pugixml.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "NGE/Media/Images/Texture.hpp"
#include "NGE/Math/Vector2.hpp"
#include "NGE/Math/Vector4.hpp"

namespace NGE
{
    namespace Media
    {
        namespace Fonts
        {
            class TextureMappedFont
            {
              public:
                TextureMappedFont(float fontSize = 16.0f, Media::Images::Texture* texture = NULL, Media::Shaders::GLSLProgram* shader = NULL);
                bool Initialize();

                void PrintString(const std::string& str, const Math::vec2f& position);
                void PrintString(const std::string& str, float x, float y);

                void PrintString(const std::string& str, const Math::vec2f& position, const Math::vec4f& color);
                void PrintString(const std::string& str, float x, float y, const Math::vec4f& color);

                // TODO: Load XML Settings
                bool LoadXMLSettings(const pugi::xml_node& node);

                void SetTexture(Media::Images::Texture* texture);
                void SetShader(Media::Shaders::GLSLProgram* shader);

              protected:
                Media::Images::Texture* texture;
                Media::Shaders::GLSLProgram* shader;
                GLuint texCoordBuffer, vertexBuffer;

                float fontSize;
                Math::vec4f defaultColor;
            };
        }
    }
}

#endif	/* TEXTUREMAPPEDFONT_HPP */

