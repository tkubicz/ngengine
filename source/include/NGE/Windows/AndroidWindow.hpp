/* 
 * File:   AndroidWindow.hpp
 * Author: tku
 *
 * Created on 26 marzec 2015, 12:42
 */

#ifndef ANDROIDWINDOW_HPP
#define ANDROIDWINDOW_HPP

#ifdef ANDROID

#include <string>
#include "NGE/Core/Core.hpp"
#include "NGE/Windows/Application.hpp"
#include "NGE/Windows/AbstractWindow.hpp"

namespace NGE {
	namespace Windows {

		/** 
		 * Class that creates an Android application window
		 */
		class AndroidWindow : public Windows::AbstractWindow {
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

			bool IsPaused();
			void SetPaused(bool value);

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

			virtual void SetApplication(Application* app);

			virtual void EnableMouseCursor(bool);
			virtual bool IsMouseCursorEnable();
			virtual Math::vec2i GetMousePosition() const;

			void setState(android_app* state) {
				this->state = state;
			}

			static void handleCmd(struct android_app* app, int32_t cmd) {
				switch (cmd) {
					case APP_CMD_INIT_WINDOW:
					{
						AndroidWindow* window = static_cast<AndroidWindow*> (app->userData);
						if (window != nullptr) {
							window->Create();
							AndroidWindow::app->OnResize(ANativeWindow_getWidth(app->window), ANativeWindow_getHeight(app->window));
							if (!AndroidWindow::app->Init()) {
								NGE::Tools::Logger::WriteErrorLog("Could not initialize application");
								return;
							}
							window->SetPaused(false);
						}
						break;
					}

					case APP_CMD_DESTROY:
					{
						AndroidWindow* window = static_cast<AndroidWindow*> (app->userData);
						if (window != nullptr) {
							window->Destroy();
						}
						break;
					}

					case APP_CMD_TERM_WINDOW:
					{
						AndroidWindow* window = static_cast<AndroidWindow*> (app->userData);
						if (window != nullptr) {
							window->Destroy();
						}
						break;
					}

					case APP_CMD_WINDOW_RESIZED:
					{
						AndroidWindow* window = static_cast<AndroidWindow*> (app->userData);
						if (window != nullptr) {
							//Tools::Logger::WriteInfoLog("Width: " + ANativeWindow_getWidth(app->window));
							//Tools::Logger::WriteInfoLog("Height: " + to_string(ANativeWindow_getHeight(app->window)));
						}
						break;
					}

					case APP_CMD_RESUME:
					{
						AndroidWindow* window = static_cast<AndroidWindow*> (app->userData);
						if (window != nullptr) {
							window->SetPaused(false);
						}
						break;
					}

					case APP_CMD_PAUSE:
					{
						AndroidWindow* window = static_cast<AndroidWindow*> (app->userData);
						if (window != nullptr) {
							window->SetPaused(true);
						}
						break;
					}
				}
			}

		  private:
			android_app* state;
			EGLDisplay eglDisplay;
			EGLContext eglContext;
			EGLSurface eglSurface;
			int width, height;
			bool initialized;
			bool isRunning;
			bool isPaused;

			static Application* app;
		};
	}
}

#endif /* ANDROID */

#endif /* ANDROIDWINDOW_HPP */