/* 
 * File:   Texture.hpp
 * Author: tku
 *
 * Created on 25 czerwiec 2012, 01:10
 */

#ifndef TEXTURE_HPP
#define	TEXTURE_HPP


#include "NGE/Core/Core.hpp"
#include "NGE/Media/Images/Image.hpp"
#include "NGE/Parsers/pugixml.hpp"

namespace NGE {
	namespace Media {
		namespace Images {

			class Texture {
			  public:
				Texture(GLuint target = GL_TEXTURE_2D);
				Texture(const Texture& copy);
				Texture& operator=(const Texture& copy);
				~Texture();

				bool loadXMLSettings(const pugi::xml_node& node, const std::string& path);

				bool load2DImage(const Image& image, GLuint clampS = GL_REPEAT, GLuint clampT = GL_REPEAT,
						GLuint magFilter = GL_LINEAR, GLuint minFilter = GL_LINEAR_MIPMAP_LINEAR,
						GLuint internalFormat = GL_RGB, GLuint format = GL_RGB, bool mipmap = true);

				bool loadCubemap(const Image& positiveX, const Image& negativeX, const Image& positiveY, const Image& negativeY,
						const Image& positiveZ, const Image& negativeZ, GLuint clampS = GL_CLAMP_TO_EDGE, GLuint clampT = GL_CLAMP_TO_EDGE,
						GLuint magFilter = GL_LINEAR, GLuint minFilter = GL_LINEAR, GLuint internalFormat = GL_RGB, GLuint format = GL_RGB);

				const bool activate(int unit = -1);
				const bool deactivate();

				void setID(GLuint textureID);
				void setTarget(GLuint target);

				const GLuint getID() const;
				const GLuint getTarget() const;
				const GLuint getHeight() const;
				const GLuint getWidth() const;
				const GLuint getSizeInBytes() const;

				void destroy();

			  protected:
				GLuint height, width;
				GLuint sizeInBytes;

			  private:
				GLuint target, id;
				int currentUnit;

				int getValidWrapMode(int clamp);
				int getValidWrapMode(const std::string& clamp);

				int getValidMagFilter(int filter);
				int getValidMagFilter(const std::string& filter);

				int getValidMinFilter(int filter);
				int getValidMinFilter(const std::string& filter);

				int getValidCubeMap(int type);
				int getValidCubeMap(const std::string& type);

				int getValidFormat(int type);
				int getValidFormat(const std::string& type);
			};
		}
	}
}

#endif	/* TEXTURE_HPP */

