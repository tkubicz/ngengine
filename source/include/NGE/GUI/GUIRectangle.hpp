/* 
 * File:   GUIRectangle.hpp
 * Author: tku
 *
 * Created on 2 czerwiec 2012, 14:41
 */

#ifndef GUIRECTANGLE_HPP
#define	GUIRECTANGLE_HPP

#include "NGE/Math/Vector2.hpp"
#include "NGE/Math/Vector4.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "NGE/Events/MouseEvent.hpp"
#include "NGE/Events/KeyboardEvent.hpp"
#include "NGE/Events/GUIEventListener.hpp"
#include "NGE/Parsers/pugixml.hpp"
#include "NGE/GUI/GUITexCoordDescriptor.hpp"
#include <string>

namespace NGE
{
    namespace GUI
    {

        class GUIRectangle
        {
        protected:
            GUIRectangle* parent;

            Math::vec4i windowBounds;
            Math::vec2i position, dimensions, lastMousePosition;
            bool mouseOver, released, focused, pressed, clicked, visible, active, update;
            int lastAction, widgetType, anchor, z;
            std::string callbackString;

        public:
            GUIRectangle(const char* callback = NULL);

            void SetCallbackString(const std::string& callback);
            void SetCallbackString(const char* callback);
            const std::string& GetCallbackString();

            virtual void SetParent(GUIRectangle* parent);
            virtual GUIRectangle* GetParent();

            bool LoadXMLSettings(const pugi::xml_node& node);

            void SetDimensions(int width, int height);
            void SetDimensions(const Math::vec2i& dimensions);
            const Math::vec2i& GetDimensions();

            void SetPosition(int xPos, int yPos);
            void SetPosition(const Math::vec2i& position);
            const Math::vec2i& GetPosition();

            virtual Math::vec2i GetCenter();

            int GetWidth();
            int GetHeight();

            void SetActive(bool active);
            bool IsActive();

            void SetAnchorPoint(const std::string& anchor);
            void SetAnchorPoint(int anchor);
            int GetAnchorPoint();

            virtual void ForceUpdate(bool update);

            void SetVisible(bool visible);
            bool IsVisible();

            bool IsAttached();
            int GetWidgetType();

            virtual void CheckMouseEvents(Events::MouseEvent& event);
            virtual void CheckKeyboardEvents(Events::KeyboardEvent& event);
            
            virtual void Update(float dt) = 0;
            virtual void Render() = 0;

            virtual GUITexCoordDescriptior* GetTexCoordsInfo(int type);
            virtual Events::GUIEventListener* GetEventsListener();
            
            virtual void EnableGUITexture();
            virtual void DisableGUITexture();
            
            virtual void SetZCoordinate(int z);
            virtual int GetZCoordinate();

            virtual const void ComputeWindowBounds();
            virtual const Math::vec4i& GetWindowBounds();

            bool EventDetected();
            bool IsMouseOver();
            bool isReleased();
            bool IsFocused();
            bool IsDragged();
            bool IsClicked();
            bool IsPressed();
        };
    }
}

#endif	/* GUIRECTANGLE_HPP */

