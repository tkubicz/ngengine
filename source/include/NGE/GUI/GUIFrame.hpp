/* 
 * File:   GUIFrame.hpp
 * Author: tku
 *
 * Created on 26 marzec 2013, 17:58
 */

#ifndef GUIFRAME_HPP
#define	GUIFRAME_HPP

#include "NGE/GUI/GUITexCoordDescriptor.hpp"
#include "NGE/GUI/GUIPanel.hpp"
#include "NGE/Parsers/pugixml.hpp"
#include "NGE/Media/Images/Texture.hpp"


namespace NGE
{
    namespace GUI
    {
        class GUIFrame : public GUIPanel
        {
        public:
            GUIFrame();
            virtual ~GUIFrame();
            
            bool LoadXMLSettings(const pugi::xml_node& node);
            
            void AddOrReplaceTexCoordsInfo(GUITexCoordDescriptior& info);
            //virtual GUITexCoordDescriptior* GetTexCoordsInfo(int type);
            
            void SetElementsTexture(const Media::Images::Texture& texture);
            void SetElementsTexture(const std::string& textureName);
            Media::Images::Texture* GetElementsTexture();
            
            void SetGUIEventListener(Events::GUIEventListener* listener);
            virtual Events::GUIEventListener* GetEventsListener();
            
            //virtual const Math::vec4i& GetWindowBounds();
            virtual void ForceUpdate(bool update);
            //virtual void EnableGUITexture();
            //virtual void DisableGUITexture();
            virtual void Update(float dt);
            virtual void Render();
            //virtual void Clear();
            
        private:
            Events::GUIEventListener* listener;
            GUITexCoordDescriptior texCoords;
            Media::Images::Texture* elementsTexture;
            Math::vec2i windowSize;
            int updateCount;
        };
    }
}

#endif	/* GUIFRAME_HPP */

