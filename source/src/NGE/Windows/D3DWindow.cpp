#ifdef _WIN32

#include "NGE/Windows/D3DWindow.hpp"
#include "NGE/Tools/Logger.hpp"

using namespace NGE::Windows;

#define ReleaseCOM(x) { if (x){ x->Release(); x = 0; } }
#define HR(x) (x)

Application* D3DWindow::app = NULL;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	D3DWindow* wnd = 0;

	switch (msg) {
		case WM_CREATE:
		{
			CREATESTRUCT* cs = (CREATESTRUCT*) lParam;
			wnd = (D3DWindow*) cs->lpCreateParams;
			return 0;
		}
	}

	if (wnd)
		return wnd->MsgProc(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

D3DWindow::D3DWindow() { }

bool D3DWindow::Create() {
	return true;
}

bool D3DWindow::Create(int width, int height, int bpp, bool fullscreen, const std::string& title, int fsaa, int openglMajor, int openglMinor) {
	return true;
}

bool D3DWindow::Create(int width, int height, const std::wstring& title, HINSTANCE hInstance) {
	appInstance = hInstance;
	mainWnd = 0;
	isRunning = true;
	isMinimized = false;
	isMaximized = false;
	isResizing = false;

	d3dDevice = 0;
	swapChain = 0;
	depthStencilBuffer = 0;
	renderTargetView = 0;
	depthStencilView = 0;
	font = 0;

	this->title = title;
	d3dDriverType = D3D10_DRIVER_TYPE_HARDWARE;
	this->width = width;
	this->height = height;

	return true;
}

bool D3DWindow::Init() {
	return true;
}

void D3DWindow::Destroy() {
	ReleaseCOM(renderTargetView);
	ReleaseCOM(depthStencilBuffer);
	ReleaseCOM(swapChain);
	ReleaseCOM(depthStencilBuffer);
	ReleaseCOM(d3dDevice);
	ReleaseCOM(font);
}

void D3DWindow::ProcessEvents() { }

void D3DWindow::SetInputCallbacks() { }

bool D3DWindow::LoadXMLSettings(pugi::xml_node& windowNode) {
	return true;
}

bool D3DWindow::IsRunning() {
	return isRunning;
}

void D3DWindow::SwapBuffers() { }

void D3DWindow::SetWindowTitle(const std::string& title) { }

std::string D3DWindow::GetWindowTitle() const {
	std::string s(title.begin(), title.end());
	return s;
}

int D3DWindow::GetWidth() {
	return width;
}

int D3DWindow::GetHeight() {
	return height;
}

NGE::Math::vec2i D3DWindow::GetSize() {
	return Math::vec2i(width, height);
}

void D3DWindow::SetWidth(int width) { }

void D3DWindow::SetHeight(int height) { }

void D3DWindow::SetSize(const NGE::Math::vec2i& size) { }

int D3DWindow::GetXPosition() {
	return 0;
}

int D3DWindow::GetYPosition() {
	return 0;
}

NGE::Math::vec2i D3DWindow::GetPosition() {
	return Math::vec2i(0, 0);
}

void D3DWindow::SetPosition(const NGE::Math::vec2i& position) { }

bool D3DWindow::GetVSync() {
	return true;
}

void D3DWindow::SetVSync(bool vsync) { }

void D3DWindow::SetApplication(Application* app) {
	this->app = app;
}

LRESULT D3DWindow::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
			// Zdarzenie WM_ACTIVATE jest wysyłane w momencie aktywacji i deaktywacji
			// okna. Domyślne zachowanie to zatrzymanie aplikacji w momencie deaktywacji
			// i ponowne uruchomienie w momencie aktywacji.
		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE) {
				isRunning = false;
			} else {
				isRunning = true;
			}
			return 0;

			// Zdarzenie wysyłane w momencie zmiany rozmiaru okna
		case WM_SIZE:
			width = LOWORD(lParam);
			height = HIWORD(lParam);
			if (d3dDevice) {
				if (wParam == SIZE_MINIMIZED) {
					isRunning = false;
					isMinimized = true;
					isMaximized = false;
				} else if (wParam == SIZE_MAXIMIZED) {
					isRunning = true;
					isMinimized = false;
					isMaximized = true;
					ResizeEvent(width, height);
				} else if (wParam == SIZE_RESTORED) {
					// Przywrócenie z minimalizacji
					if (isMinimized) {
						isRunning = true;
						isMinimized = false;
						ResizeEvent(width, height);
					}						// Przywrócenie z maksymalizacji
					else if (isMaximized) {
						isRunning = true;
						isMaximized = false;
						ResizeEvent(width, height);
					} else if (isResizing) {

					} else {
						ResizeEvent(width, height);
					}
				}
			}
			return 0;

			// Zdarzenie wysyłane gdy użytkownik zaczyna zmieniać rozmiar okna
		case WM_ENTERSIZEMOVE:
			isRunning = false;
			isResizing = true;
			return 0;

			// Zdarzenie wysyłane gdy użytkownik zwolni pasek przesuwania.
		case WM_EXITSIZEMOVE:
			isRunning = true;
			isResizing = false;
			ResizeEvent(width, height);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_MENUCHAR:
			// Wyłączenie dzwięku przy alt+enter
			return MAKELRESULT(0, MNC_CLOSE);

			// Przechwycenie tego zdarzenia w celu zapobięgnięciu zmniejszenia
			// rozmiaru okna na mniej niż 200x200.
		case WM_GETMINMAXINFO:
			((MINMAXINFO*) lParam)->ptMinTrackSize.x = 200;
			((MINMAXINFO*) lParam)->ptMinTrackSize.y = 200;
			return 0;
	}

	return DefWindowProc(mainWnd, msg, wParam, lParam);
}

void D3DWindow::InitMainWindow() {
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// TODO:
	//wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = appInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"D3DWndClassName";

	if (!RegisterClass(&wc)) {
		Tools::Logger::WriteErrorLog("D3DWindow -> RegisterClass failed.");
		PostQuitMessage(0);
	}

	// Obliczenie rozmiaru okna
	RECT r = {0, 0, width, height};
	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, false);
	int width = r.right - r.left;
	int height = r.bottom - r.top;

	mainWnd = CreateWindowW(L"D3DWndClassName", title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, appInstance, this);

	if (!mainWnd) {
		Tools::Logger::WriteErrorLog("D3DWindow -> CreateWindow failed.");
		PostQuitMessage(0);
	}

	ShowWindow(mainWnd, SW_SHOW);
	UpdateWindow(mainWnd);
}

void D3DWindow::InitDirect3D() {
	// Wypełnienie DXGI_SWAP_CHAIN_DESC
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Bez multisamplingu
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = mainWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// Utworzenie urządzenia
	UINT createDeviceFlags = 0;

	HR(D3D10CreateDeviceAndSwapChain(
			0, // Domyślny adapter
			d3dDriverType,
			0, // Bez urządzenia programowego (software device)
			createDeviceFlags,
			D3D10_SDK_VERSION,
			&sd,
			&swapChain,
			&d3dDevice));

	ResizeEvent(width, height);
}

void D3DWindow::ResizeEvent(int width, int height) { }

void D3DWindow::KeyPressedEvent(int key, int action) { }

void D3DWindow::MousePressedEvent(int button, int action) { }

void D3DWindow::MouseMoveEvent(int x, int y) { }

void D3DWindow::MouseScrollEvent(int action) { }


#endif
