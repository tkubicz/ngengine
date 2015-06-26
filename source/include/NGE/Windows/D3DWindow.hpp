/* 
 * File:   D3DWindow.hpp
 * Author: tku
 *
 * Created on 3 luty 2014, 12:59
 */

#ifndef D3DWINDOW_HPP
#define	D3DWINDOW_HPP

#ifdef _WIN32

#include <windows.h>
#include <D3DX10.h>

#include "NGE/Windows/Window.h"
#include "NGE/Windows/Application.h"
#include "NGE/Parsers/pugixml.hpp"

namespace NGE {
	namespace Windows {

		class D3DWindow : public Windows::Window {
		  public:
			D3DWindow();

			virtual bool Create();
			virtual bool Create(int width, int height, int bpp, bool fullscreen, const std::string& title, int fsaa, int openglMajor, int openglMinor);
			virtual bool Create(int width, int height, const std::wstring& title, HINSTANCE hInstance);

			virtual bool Init();
			virtual void Destroy();
			virtual void ProcessEvents();
			virtual void SetInputCallbacks();

			virtual bool LoadXMLSettings(pugi::xml_node& windowNode);

			virtual bool IsRunning();
			virtual void SwapBuffers();

			virtual void SetWindowTitle(const std::string& title);
			virtual std::string GetWindowTitle() const;

			virtual int GetWidth();
			virtual int GetHeight();
			virtual Math::vec2i GetSize();

			virtual void SetWidth(int width);
			virtual void SetHeight(int height);
			virtual void SetSize(const NGE::Math::vec2i& size);

			virtual int GetXPosition();
			virtual int GetYPosition();
			virtual Math::vec2i GetPosition();
			virtual void SetPosition(const NGE::Math::vec2i& position);

			virtual bool GetVSync();
			virtual void SetVSync(bool vsync);

			virtual void SetApplication(Application* app);

			LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);

		  protected:
			static void ResizeEvent(int width, int height);
			static void KeyPressedEvent(int key, int action);
			static void MousePressedEvent(int button, int action);
			static void MouseMoveEvent(int x, int y);
			static void MouseScrollEvent(int action);

			void InitMainWindow();
			void InitDirect3D();

			static Application* app;

			int width, height;
			std::wstring title;
			Math::vec2i position;

			bool isRunning, isFullscreen, isMinimized, isMaximized, isResizing;

			HINSTANCE appInstance;
			HWND mainWnd;

			ID3D10Device* d3dDevice;
			IDXGISwapChain* swapChain;
			ID3D10Texture2D* depthStencilBuffer;
			ID3D10RenderTargetView* renderTargetView;
			ID3D10DepthStencilView* depthStencilView;
			ID3DX10Font* font;

			D3D10_DRIVER_TYPE d3dDriverType;
		};
	}
}

#endif

#endif	/* D3DWINDOW_HPP */

