/* 
 * File:   TextureManager.hpp
 * Author: tku
 *
 * Created on 11 March 2013, 17:41
 */

#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <map>
#include <string>
#include <pugixml.hpp>

#include "NGE/Core/Singleton.hpp"
#include "NGE/Events/IEventData.hpp"
#include "NGE/Events/EventDelegate.hpp"
#include "NGE/Media/Images/Texture.hpp"

namespace NGE {
    namespace Media {
        namespace Images {

            class TextureManager : public Core::Singleton<TextureManager> {
                friend class NGE::Core::Singleton<TextureManager>;

                typedef std::map<NGE::Events::EventType, NGE::Events::EventDelegate> EventDelegateMap;
                typedef std::pair<NGE::Events::EventType, NGE::Events::EventDelegate> EventDelegatePair;

              private:
                std::map<std::string, Texture*> textures;
                EventDelegateMap eventDelegates;

              private:
                TextureManager() {}
                ~TextureManager() {}

              public:
                void Initialise();
                void Deinitialise();

                bool AddTexture(const std::string& name, Texture* texture);
                bool LoadTexture(const pugi::xml_node& node);

                void LoadTextureListener(NGE::Events::IEventDataPtr event);
                void GetTextureListener(NGE::Events::IEventDataPtr event);

                Texture* GetTexture(const std::string& name);
                Texture* GetTexture(const pugi::xml_node& node);

                unsigned int GetTextureCount();
            };
        }
    }
}

#endif /* TEXTUREMANAGER_HPP */

