/* 
 * File:   AndroidWindow.hpp
 * Author: tku
 *
 * Created on 26 marzec 2015, 12:42
 */

#ifndef ANDROIDWINDOW_HPP
#define ANDROIDWINDOW_HPP

#include <string>
#include "NGE/Windows/Window.hpp"
#include "NGE/Math/Vector2.hpp"
#include "NGE/ThirdPart/android_native_app_glue.h"

namespace NGE {
	namespace Windows {

		/** 
		 * Class that creates an Android application window
		 */
		class AndroidWindow : public Windows::Window {
		  public:
			AndroidWindow();

			virtual bool Create();
			virtual bool Create(int width, int height, int bpp, bool fullscreen, const std::string& title, int fsaa = 0, int openglMajor = 3, int openglMinor = 0);

			virtual bool CreateSlave(bool visible);

			virtual bool Init();
			virtual void Destroy();

			virtual void SetMainWindowContext();
			virtual void SetSlaveWindowContext();

			virtual void ProcessEvents();
			virtual void SetInputCallbacks();

			virtual bool LoadXMLSettings(pugi::xml_node& windowNode);

			virtual bool IsRunning();
			virtual void SwapBuffers();

			virtual void SetWindowTitle(const std::string& title);
			virtual std::string GetWindowTitle() const;

			virtual int GetWidth() const;
			virtual int GetHeight() const;
			virtual Math::vec2i GetSize() const;

			virtual void SetWidth(const int width);
			virtual void SetHeight(const int height);
			virtual void SetSize(const Math::vec2i& size);

			virtual int GetXPosition() const;
			virtual int GetYPosition() const;
			virtual Math::vec2i GetPosition() const;
			virtual void SetPosition(const Math::vec2i& position);

			virtual bool GetVSync();
			virtual void SetVSync(bool vsync);

			virtual void EnableMouseCursor(bool);
			virtual bool IsMouseCursorEnable();
			virtual Math::vec2i GetMousePosition() const;

		  private:
			struct android_app* androidApplication;
			bool isRunning;
		};
	}
}

#endif /* ANDROIDWINDOW_HPP */