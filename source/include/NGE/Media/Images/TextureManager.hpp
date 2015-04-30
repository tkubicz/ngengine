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
#include "NGE/Parsers/pugixml.hpp"

namespace NGE
{
    namespace Media
    {
        namespace Images
        {
            class TextureManager
            {
              public:
                void Initialize();
                void Deinitialize();

                bool AddTexture(const std::string& name, Texture* texture);
                
                bool LoadTexture(const pugi::xml_node& node);
                Texture* GetTexture(const std::string& name);
                Texture* GetTexture(const pugi::xml_node& node);

                int GetTextureCount();

              private:
                std::map<std::string, Texture*> textures;
            };
        }
    }
}

#endif	/* TEXTUREMANAGER_HPP */

