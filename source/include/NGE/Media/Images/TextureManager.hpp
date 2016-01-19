/* 
 * File:   TextureManager.hpp
 * Author: tku
 *
 * Created on 11 marzec 2013, 17:41
 */

#ifndef TEXTUREMANAGER_HPP
#define	TEXTUREMANAGER_HPP

#include <map>
#include <string>

#include "NGE/Media/Images/Texture.hpp"
#include <pugixml.hpp>

namespace NGE {
	namespace Media {
		namespace Images {

			class TextureManager {
			  public:
				void initialize();
				void deinitialize();

				bool addTexture(const std::string& name, Texture* texture);

				bool loadTexture(const pugi::xml_node& node);
				Texture* getTexture(const std::string& name);
				Texture* getTexture(const pugi::xml_node& node);

				int getTextureCount();

			  private:
				std::map<std::string, Texture*> textures;
			};
		}
	}
}

#endif	/* TEXTUREMANAGER_HPP */

