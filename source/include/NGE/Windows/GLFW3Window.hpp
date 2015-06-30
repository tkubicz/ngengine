/* 
 * File:   GLFW3Window.h
 * Author: tku
 *
 * Created on 22 wrzesień 2014, 15:08
 */

#ifndef GLFW3WINDOW_HPP
#define	GLFW3WINDOW_HPP

#include "NGE/Core/Core.hpp"
#include "NGE/Parsers/pugixml.hpp"
#include "NGE/Windows/Application.hpp"
#include "NGE/Windows/AbstractWindow.hpp"

namespace NGE {
	namespace Windows {

		class GLFWwindow;

		/**
		 * Class that implements GLFW3 window managment under diffrent operation
		 * systems. It is based on GLFWWindow class, which was used to use old
		 * GLFW2 library.
		 */
		class GLFW3Window : public Windows::AbstractWindow {
		  public:
			GLFW3Window();

			virtual bool Create();
			virtual bool Create(int width, int height, int bpp, bool fullscreen, const std::string& title, int fsaa = 0, int openglMajor = 3, int openglMinor = 0);

			virtual bool CreateSlave(bool visible);

			virtual bool Init();
			virtual void Destroy();

			virtual void SetMainWindowContext();
			virtual void SetSlaveWindowContext();

			virtual void ProcessEvents();
			virtual void SetInputCallbacks();

			bool LoadXMLSettings(pugi::xml_node& windowNode);

			bool IsRunning();
			void SwapBuffers();

			void SetWindowTitle(const std::string& title);
			std::string GetWindowTitle() const;

			int GetWidth() const;
			int GetHeight() const;
			Math::vec2i GetSize() const;

			void SetWidth(const int width);
			void SetHeight(const int height);
			void SetSize(const Math::vec2i& size);

			int GetXPosition() const;
			int GetYPosition() const;
			Math::vec2i GetPosition() const;
			void SetPosition(const Math::vec2i& position);

			bool GetVSync();
			void SetVSync(bool vsync);

			void SetApplication(Application* app);

			virtual void EnableMouseCursor(bool value);
			virtual bool IsMouseCursorEnable();

			Math::vec2i GetMousePosition() const;
			void SetMousePosition(const int x, const int y);

		  protected:
			static void ResizeEvent(GLFWwindow* window, int width, int height);
			static void KeyPressedEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
			static void MousePressedEvent(GLFWwindow* window, int button, int action, int mods);
			static void MouseMoveEvent(GLFWwindow* window, double x, double y);
			static void MouseScrollEvent(int action);

			GLFWwindow* window;
			GLFWwindow* slaveWindow;

			static Application* app;
			static int lastMouseAction, lastMouseButton;
			static int modifier;

			bool isRunning, isFullscreen, isMouseCursor, isVSync;
			int width, height, redbits, greenbits, bluebits, alphabits, depthbits, stencilbits, fsaa, openglMajor, openglMinor;
			std::string title;
			Math::vec2i position;
		};
	}
}

#endif	/* GLFW3WINDOW_HPP */

