/* 
 * File:   Window.h
 * Author: tku
 *
 * Created on 7 styczeń 2013, 00:58
 */

#ifndef ABSTRACTWINDOW_HPP
#define	ABSTRACTWINDOW_HPP

#include <string>

#include "NGE/Parsers/pugixml.hpp"
#include "NGE/Math/Vector2.hpp"

namespace NGE {
	namespace Windows {

		class Application;
		
		class AbstractWindow {
		  public:
			virtual bool Create() = 0;

			// TODO: Do modyfikacji - taka metoda nie powinna istnieć w silniku który obsługuje również DirectX
			virtual bool Create(int width, int height, int bpp, bool fullscreen, const std::string& title, int fsaa, int openglMajor, int openglMinor) = 0;

			virtual bool CreateSlave(bool visible) = 0;

			virtual bool Init() = 0;
			virtual void Destroy() = 0;

			virtual void SetMainWindowContext() = 0;
			virtual void SetSlaveWindowContext() = 0;

			virtual void ProcessEvents() = 0;
			virtual void SetInputCallbacks() = 0;

			virtual bool LoadXMLSettings(pugi::xml_node& node) = 0;

			virtual bool IsRunning() = 0;
			virtual void SwapBuffers() = 0;

			virtual void SetWindowTitle(const std::string& title) = 0;
			virtual std::string GetWindowTitle() const = 0;

			virtual int GetWidth() const = 0;
			virtual void SetWidth(int) = 0;

			virtual int GetHeight() const = 0;
			virtual void SetHeight(int) = 0;

			virtual Math::vec2i GetSize() const = 0;
			virtual void SetSize(const Math::vec2i&) = 0;

			virtual int GetXPosition() const = 0;
			virtual int GetYPosition() const = 0;
			virtual Math::vec2i GetPosition() const = 0;
			virtual void SetPosition(const Math::vec2i&) = 0;

			virtual bool GetVSync() = 0;
			virtual void SetVSync(bool vsync) = 0;

			virtual void SetApplication(Application* app) = 0;

			virtual void EnableMouseCursor(bool) = 0;
			virtual bool IsMouseCursorEnable() = 0;
			virtual Math::vec2i GetMousePosition() const = 0;
		};
	}
}

#endif	/* ABSTRACTWINDOW_HPP */

