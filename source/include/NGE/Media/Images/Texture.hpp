/* 
 * File:   Texture.hpp
 * Author: tku
 *
 * Created on 25 czerwiec 2012, 01:10
 */

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <pugixml.hpp>
#include "NGE/Core/Core.hpp"
#include "NGE/Media/Images/Image.hpp"

namespace NGE {
	namespace Media {
		namespace Images {

			class Texture {
			  public:
				Texture(GLuint target = GL_TEXTURE_2D);
				Texture(const Texture& copy);
				Texture& operator=(const Texture& copy);
				~Texture();

				bool LoadXMLSettings(const pugi::xml_node& node, const std::string& path);

				bool Load2DImage(Image*, GLuint clampS = GL_REPEAT, GLuint clampT = GL_REPEAT,
						GLuint magFilter = GL_LINEAR, GLuint minFilter = GL_LINEAR_MIPMAP_LINEAR,
						GLuint internalFormat = GL_RGB, GLuint format = GL_RGB, bool mipmap = true);

				bool LoadCubemap(Image* positiveX, Image* negativeX, Image* positiveY, Image* negativeY,
						Image* positiveZ, Image* negativeZ, GLuint clampS = GL_CLAMP_TO_EDGE, GLuint clampT = GL_CLAMP_TO_EDGE,
						GLuint magFilter = GL_LINEAR, GLuint minFilter = GL_LINEAR, GLuint internalFormat = GL_RGB, GLuint format = GL_RGB);

				const bool Activate(int unit = -1);
				const bool Deactivate();

				void SetID(GLuint textureID);
				void SetTarget(GLuint target);

				const GLuint GetID() const;
				const GLuint GetTarget() const;
				const GLuint GetHeight() const;
				const GLuint GetWidth() const;
				const GLuint GetSizeInBytes() const;

				void Unload();

			  protected:
				GLuint height, width;
				GLuint sizeInBytes;

			  private:
				GLuint target, id;
				int currentUnit;

				bool LoadTexture2D();
				bool LoadTextureCubeMap();

				int GetValidWrapMode(int clamp);
				int GetValidWrapMode(const std::string& clamp);

				int GetValidMagFilter(int filter);
				int GetValidMagFilter(const std::string& filter);

				int GetValidMinFilter(int filter);
				int GetValidMinFilter(const std::string& filter);

				int GetValidCubeMap(int type);
				int GetValidCubeMap(const std::string& type);

				int GetValidFormat(int type);
				int GetValidFormat(const std::string& type);

				/**
				 * Get internal texture type id from input string.
				 * @param textureType Can be "png", "jpeg" or "tga"
				 * @return -1 in case of unsupported type. 0 >= if type is supported.
				 */
				int GetTextureType(const std::string& textureType);

				Image* CreateImagePointer(const std::string& type);

				template <typename T> void DeleteImagePointer(T pointer) {
					delete pointer;
				}

				template <typename T, typename... Args> void DeleteImagePointer(T first, Args... args) {
					delete first;
					DeleteImagePointer(args...);
				}
			};
		}
	}
}

#endif /* TEXTURE_HPP */

