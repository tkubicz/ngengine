/* 
 * File:   GUIClippedRectangle.hpp
 * Author: tku
 *
 * Created on 12 listopad 2012, 02:06
 */

#ifndef GUICLIPPEDRECTANGLE_HPP
#define	GUICLIPPEDRECTANGLE_HPP

#include "NGE/GUI/GUIRectangle.hpp"
#include "NGE/Media/Images/Texture.hpp"
#include "NGE/Math/Vector3.hpp"
#include "NGE/Math/Vector4.hpp"

namespace NGE
{
    namespace GUI
    {

        class GUIClippedRectangle
        {
        protected:
            Media::Images::Texture* texture;
            Media::Shaders::GLSLProgram* shader;

            Math::vec4f textureRectangle;
            Math::vec4f backgroundColor;
            Math::vec3f bordersColor;
            Math::vec4f vertices[7];
            
            GLuint vertexBuffer;

            bool drawBackground;
            bool drawBounds;
            int clipSize;

        public:
            GUIClippedRectangle(int clipSize = 5);
            virtual ~GUIClippedRectangle();
            
            void EnableBackgroundColor(bool enable);
            bool IsBackgroundColorOn();
            
            void ComputeClippedBounds(const Math::vec4i& windowBounds);
            
            void SetClipSize(int clipSize);
            int GetClipSize();
            
            void SetVisibleBounds(bool visible);
            bool BoundsVisible();
            
            void SetBackgroundColor(float r, float g, float b, float alpha);
            void SetBackgroundColor(const Math::vec4f& color);
            const Math::vec4f& GetBackgroundColor();
            const Math::vec2i& GetVertices();
            
            void SetBordersColor(const Math::vec3f& color);
            void SetBordersColor(float r, float g, float b);
            const Math::vec3f& GetBordersColor();
            const Math::vec2f& GetTexCoords();
            
            void SetTextureRectangle(float x, float y, float z, float w);
            void SetTextureRectangle(const Math::vec4f& textureRectangle);
            const Math::vec4f& GetTextureRectangle();
            
            Media::Images::Texture* GetTexture();
            void SetTexture(Media::Images::Texture* texture);
            
            Media::Shaders::GLSLProgram* GetShader();
            void SetShader(Media::Shaders::GLSLProgram* shader);
            
            virtual void RenderClippedBounds();
            virtual bool LoadXMLClippedRectangleInfo(const pugi::xml_node& node);
        };
    }
}

#endif	/* GUICLIPPEDRECTANGLE_HPP */

