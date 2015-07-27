#ifdef ANDROID
#include "NGE/Core/Core.hpp"
#include "NGE/Windows/AndroidWindow.hpp"
#include "NGE/Media/Files/File.hpp"

using namespace NGE::Windows;

Application* AndroidWindow::app = NULL;

AndroidWindow::AndroidWindow() {
	isRunning = true;
	isPaused = true;
}

bool AndroidWindow::Create() {
	eglDisplay = EGL_NO_DISPLAY;
	eglContext = EGL_NO_CONTEXT;
	eglSurface = EGL_NO_SURFACE;
	width = height = 0;

	// Initialize OpenGL ES and EGL

	// Here we specify the attributes of the desired configuration.
	// Below, we select an EGLConfig with at least 8 bits per colour
	// component compatible with on-screen windows.
	const EGLint attribs[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};

	EGLint format;
	EGLint numConfigs;
	EGLConfig config;

	eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(eglDisplay, NULL, NULL);

	Tools::Logger::WriteInfoLog("eglDisplay initialized");

	// Here the application chooses the configuration it desires. We pick
	// the first EGLConfig that matches criteria.
	// TODO: Reconsider this.
	eglChooseConfig(eglDisplay, attribs, &config, 1, &numConfigs);

	// EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is 
	// guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	// As soon as we picked a EGLConfig, we can safely reconfgure the
	// ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID.
	eglGetConfigAttrib(eglDisplay, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(state->window, 0, 0, format);

	eglSurface = eglCreateWindowSurface(eglDisplay, config, state->window, NULL);

	EGLint contextAttribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	eglContext = eglCreateContext(eglDisplay, config, NULL, contextAttribs);

	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);

	eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &width);
	eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &height);

	initialized = true;
	isPaused = false;

	return true;
}

bool AndroidWindow::Create(int width, int height, int bpp, bool fullscreen, const std::string& title, int fsaa, int openglMajor, int openglMinor) { }

bool AndroidWindow::CreateSlave(bool visible) {
	return false;
}

bool AndroidWindow::Init() {
	Tools::Logger::WriteInfoLog("AndroidWindow init invoked");
	state->onAppCmd = AndroidWindow::handleCmd;
    state->onInputEvent = AndroidWindow::handleInput;
	state->userData = static_cast<void*> (this);
	
	if (state->activity->assetManager == nullptr) {
		Tools::Logger::WriteErrorLog("AssetManager is null");
	} else {
		Media::Files::File::SetAssetManager(state->activity->assetManager);
	}
}

void AndroidWindow::Destroy() {
	initialized = false;

	if (eglDisplay != EGL_NO_DISPLAY) {
		eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (eglContext != EGL_NO_CONTEXT) {
			eglDestroyContext(eglDisplay, eglContext);
		}
		if (eglSurface != EGL_NO_SURFACE) {
			eglDestroySurface(eglDisplay, eglSurface);
		}
		eglTerminate(eglDisplay);
	}

	eglDisplay = EGL_NO_DISPLAY;
	eglContext = EGL_NO_CONTEXT;
	eglSurface = EGL_NO_SURFACE;
}

void AndroidWindow::SetMainWindowContext() { }

void AndroidWindow::SetSlaveWindowContext() { }

void AndroidWindow::ProcessEvents() {
	int events;
	struct android_poll_source* source;
	int ident = ALooper_pollAll(0, 0, &events, (void**) &source);
	if (ident >= 0) {
		if (source) {
			source->process(state, source);
		}

		if (state->destroyRequested) {
			isRunning = false;
		}
	}
}

void AndroidWindow::SetInputCallbacks() { }

bool AndroidWindow::LoadXMLSettings(pugi::xml_node& windowNode) {
	return true;
}

bool AndroidWindow::IsRunning() {
	return isRunning;
}

void AndroidWindow::SetPaused(bool value) {
	Tools::Logger::WriteInfoLog("SetPaused with value: " + to_string(value));
	isPaused = value;
}

bool AndroidWindow::IsPaused() {
	return isPaused;
}

void AndroidWindow::SwapBuffers() {
	eglSwapBuffers(eglDisplay, eglSurface);
}

void AndroidWindow::SetWindowTitle(const std::string& title) { }

std::string AndroidWindow::GetWindowTitle() const {
	return "window title";
}

int AndroidWindow::GetWidth() const {
	return width;
}

int AndroidWindow::GetHeight() const {
	return height;
}

NGE::Math::vec2i AndroidWindow::GetSize() const {
	return Math::vec2i(width, height);
}

void AndroidWindow::SetWidth(const int width) {
	this->width = width;
}

void AndroidWindow::SetHeight(const int height) {
	this->height = height;
}

void AndroidWindow::SetSize(const Math::vec2i& size) { }

int AndroidWindow::GetXPosition() const { }

int AndroidWindow::GetYPosition() const { }

NGE::Math::vec2i AndroidWindow::GetPosition() const {
	return Math::vec2i(1, 1);
}

void AndroidWindow::SetPosition(const Math::vec2i& position) { }

bool AndroidWindow::GetVSync() {
	return true;
}

void AndroidWindow::SetVSync(bool vsync) { }

void AndroidWindow::SetApplication(Application* app) {
	this->app = app;
	app->SetWindow(this);
}

void AndroidWindow::EnableMouseCursor(bool) { }

bool AndroidWindow::IsMouseCursorEnable() {
	return false;
}

NGE::Math::vec2i AndroidWindow::GetMousePosition() const {
	return Math::vec2i(0, 0);
}

#endif