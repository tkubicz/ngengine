#ifdef USE_GLFW
#include "NGE/Core/Core.hpp"
#include "NGE/Windows/GLFW3Window.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"
#include "NGE/Events/EventManager.hpp"

#include <iostream>
#include <sstream>
#include <cmath>

using namespace NGE::Windows;

Application* GLFW3Window::app = NULL;

GLFW3Window::GLFW3Window() {
    isRunning = true;
    isMouseCursor = true;

    lastMouseAction = Events::NOACTION;
    lastMouseButton = Events::NOBUTTON;
}

bool GLFW3Window::Create() {
    glfwWindowHint(GLFW_SAMPLES, fsaa);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openglMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openglMinor);

    GLFWmonitor* primaryMonitor = nullptr;
    if (isFullscreen) {
        primaryMonitor = glfwGetPrimaryMonitor();
    }
    window = glfwCreateWindow(width, height, title.c_str(), primaryMonitor, NULL);
    if (!window) {
        log_error("Could not create GLFW3 window");
        return false;
    }

    SetVSync(isVSync);

    return true;
}

bool GLFW3Window::Create(int width, int height, int bpp, bool fullscreen, const std::string& title, int fsaa, int openglMajor, int openglMinor) {
    glfwWindowHint(GLFW_SAMPLES, fsaa);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openglMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openglMinor);

    GLFWmonitor* primaryMonitor = nullptr;
    if (fullscreen) {
        primaryMonitor = glfwGetPrimaryMonitor();
    }
    window = glfwCreateWindow(width, height, title.c_str(), primaryMonitor, NULL);
    if (!window) {
        log_error("Could not create GLFW3 window");
        return false;
    }

    this->width = width;
    this->height = height;
    this->title = title;
    this->depthbits = depthbits;
    this->isFullscreen = fullscreen;
    this->isVSync = false;
    this->fsaa = fsaa;
    this->openglMajor = openglMajor;
    this->openglMinor = openglMinor;

    SetVSync(false);

    return true;
}

bool GLFW3Window::CreateSlave(bool visible) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openglMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openglMinor);

    if (visible)
        glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    else
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

    slaveWindow = glfwCreateWindow(width, height, title.c_str(), 0, window);

    if (!slaveWindow) {
        log_error("Could not create GLFW3 slave window");
        return false;
    } else {
        log_error("Slave window created");
        return true;
    }
}

bool GLFW3Window::Init() {
    glfwMakeContextCurrent(window);

    // FIXIT: Without this line, app throws an error while using glGenVertexArrays
    glewExperimental = GL_TRUE;

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        Destroy();
        return false;
    }

    // FIXIT: Discard the GL_INVALID_ENUM error while using GLFW and GLEW.
    glGetError();

    return true;
}

void GLFW3Window::Destroy() {
    glfwDestroyWindow(window);
}

void GLFW3Window::SetMainWindowContext() {
    glfwMakeContextCurrent(window);
}

void GLFW3Window::SetSlaveWindowContext() {
    glfwMakeContextCurrent(slaveWindow);
}

void GLFW3Window::ProcessEvents() {
    isRunning = !glfwGetKey(window, GLFW_KEY_ESCAPE) && !glfwWindowShouldClose(window);
    glfwPollEvents();
}

void GLFW3Window::SetInputCallbacks() {
    glfwSetWindowSizeCallback(window, GLFW3Window::ResizeEvent);
    glfwSetKeyCallback(window, GLFW3Window::KeyPressedEvent);
    glfwSetCursorPosCallback(window, GLFW3Window::MouseMoveEvent);
    glfwSetMouseButtonCallback(window, GLFW3Window::MousePressedEvent);
}

bool GLFW3Window::LoadXMLSettings(pugi::xml_node& windowNode) {
    if (windowNode.attribute("name").empty()) {
        log_error("Name attribute cannot be empty");
        return false;
    } else
        this->title = windowNode.attribute("name").as_string();

    this->width = windowNode.attribute("width").as_int();
    this->height = windowNode.attribute("height").as_int();
    this->position.x = windowNode.attribute("x").as_int();
    this->position.y = windowNode.attribute("y").as_int();
    this->redbits = windowNode.attribute("redbits").as_int();
    this->greenbits = windowNode.attribute("greenbits").as_int();
    this->bluebits = windowNode.attribute("bluebits").as_int();
    this->alphabits = windowNode.attribute("alphabits").as_int();
    this->stencilbits = windowNode.attribute("stencilbits").as_int();
    this->depthbits = windowNode.attribute("depthbits").as_int();
    this->isFullscreen = windowNode.attribute("fullscreen").as_bool();
    this->isVSync = windowNode.attribute("vsync").as_bool();
    this->fsaa = windowNode.attribute("fsaa").as_int(0);
    this->openglMajor = windowNode.attribute("opengl_major").as_int(3);
    this->openglMinor = windowNode.attribute("opengl_minor").as_int(0);

    return true;
}

bool GLFW3Window::IsRunning() {
    return isRunning;
}

void GLFW3Window::SwapBuffers() {
    glfwSwapBuffers(window);
}

void GLFW3Window::SetWindowTitle(const std::string& title) {
    this->title = title;
    glfwSetWindowTitle(window, title.c_str());
}

std::string GLFW3Window::GetWindowTitle() const {
    return title;
}

int GLFW3Window::GetHeight() const {
    return height;
}

int GLFW3Window::GetWidth() const {
    return width;
}

NGE::Math::vec2i GLFW3Window::GetSize() const {
    return Math::vec2i(width, height);
}

void GLFW3Window::SetHeight(const int height) {
    this->height = height;
}

void GLFW3Window::SetWidth(const int width) {
    this->width = width;
}

void GLFW3Window::SetSize(const Math::vec2i& size) {
    this->width = size.x;
    this->height = size.y;
}

int GLFW3Window::GetXPosition() const {
    return position.x;
}

int GLFW3Window::GetYPosition() const {
    return position.y;
}

NGE::Math::vec2i GLFW3Window::GetPosition() const {
    return position;
}

void GLFW3Window::SetPosition(const Math::vec2i& position) {
    this->position = position;
}

bool GLFW3Window::GetVSync() {
    return isVSync;
}

void GLFW3Window::SetVSync(bool vsync) {
    this->isVSync = vsync;
    // TODO: Fix this!
    glfwSwapInterval(60);
}

void GLFW3Window::SetApplication(Application* app) {
    this->app = app;
    app->SetWindow(this);
}

void GLFW3Window::EnableMouseCursor(bool value) {
    isMouseCursor = value;
    isMouseCursor ? glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL) : glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

bool GLFW3Window::IsMouseCursorEnable() {
    return isMouseCursor;
}

NGE::Math::vec2i GLFW3Window::GetMousePosition() const {
    // TODO: Fix that.
    return Math::vec2i(0, 0);
}

void GLFW3Window::SetMousePosition(const int x, const int y) {
    glfwSetCursorPos(window, x, y);
}

void GLFW3Window::ResizeEvent(GLFWwindow* window, int width, int height) {
    GLFW3Window::app->OnResize(width, height);
}

int GLFW3Window::modifier = NGE::Events::MODIFIER_NONE;

void GLFW3Window::KeyPressedEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // TODO: Most likely refactor this code due change from glfw2 to glfw3.
    if (key == Events::KEY_LSHIFT || key == Events::KEY_RSHIFT || key == Events::KEY_LCTRL ||
        key == Events::KEY_RCTRL || key == Events::KEY_LALT || key == Events::KEY_RALT) {
        if (action == 1)
            modifier |= (int) std::pow(2.0, key % 11);
        else
            modifier &= ~(int) (std::pow(2.0, key % 11));
    }

    Events::KeyboardEvent event(key, action, modifier);
    GLFW3Window::app->OnKeyPressed(event);
}

int GLFW3Window::lastMouseAction = NGE::Events::NOACTION;
int GLFW3Window::lastMouseButton = NGE::Events::NOBUTTON;

void GLFW3Window::MousePressedEvent(GLFWwindow* window, int button, int action, int mods) {
    // TODO: Refactor this code, due the change from version 2 to 3.
    static int viewport[4], buttonId, actionId;
    static double x, y;

    glfwGetCursorPos(window, &x, &y);
    glGetIntegerv(GL_VIEWPORT, viewport);

    switch (button) {
        case -1:
            buttonId = lastMouseButton;
            break;
        case 0:
            buttonId = Events::BUTTON1;
            break;
        case 1:
            buttonId = Events::BUTTON2;
            break;
        case 3:
            buttonId = Events::BUTTON3;
            break;
        default:
            buttonId = Events::NOBUTTON;
    }

    switch (action) {
        case -1:
            if (actionId == Events::RELEASED) {
                actionId = Events::NOACTION;
                buttonId = Events::NOBUTTON;
            } else
                actionId = lastMouseAction;
            break;
        case 0:
            actionId = Events::RELEASED;
            break;
        case 1:
            actionId = Events::PRESSED;
            break;
        default:
            actionId = Events::NOACTION;
    }

    GLFW3Window::lastMouseButton = buttonId;
    GLFW3Window::lastMouseAction = actionId;

    Events::MouseEvent event(buttonId, actionId, x, y, viewport[3] - y, Events::NOSCROLLING);

    if (button == -1 && action == -1)
        event.SetMoved(true);

    //    Events::IEventDataPtr eventPtr(new Events::MouseEvent(buttonId, actionId, x, y, viewport[3] - y, Events::NOSCROLLING));
    //    Events::EventManager::GetInstance().TriggerEvent(eventPtr);

    GLFW3Window::app->OnMouse(event);
}

void GLFW3Window::MouseMoveEvent(GLFWwindow* window, double x, double y) {
    //GLFWWindow::app->OnMouseDrag(x, y);
    GLFW3Window::MousePressedEvent(window, -1, -1, 0);
}

#endif