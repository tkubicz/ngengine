#ifdef ANDROID
#include <GLES3/gl3.h>
#else 
#include <GL/glew.h>
#endif


#include "NGE/Windows/Application.hpp"
#include "NGE/Rendering/Renderer.hpp"
#include "NGE/Math/Vector2.hpp"
#include "NGE/Math/Matrix4.hpp"

using namespace NGE::Windows;

Application::Application() { }

Application::~Application() { }

bool Application::Init() {
    return true;
}

void Application::Prepare(float dt) { }

void Application::Render() { }

void Application::Shutdown() { }

void Application::OnKeyPressed(NGE::Events::KeyboardEvent& event) { }

void Application::OnMouse(NGE::Events::MouseEvent& event) { }

void Application::OnMouseDrag(int x, int y) { }

void Application::OnTouch(Events::TouchEvent& event) { }

void Application::OnResize(int width, int height) {

    Tools::Logger::WriteInfoLog("Application::OnResize - width/height: " + to_string(width) + "/" + to_string(height));

    window->SetSize(Math::vec2i(width, height));
    glViewport(0, 0, width, height);

    Rendering::Renderer::GetInstance().GetMatrixStack().SetMatrixMode(PROJECTION_MATRIX);
    Rendering::Renderer::GetInstance().GetMatrixStack().Identity();

    Rendering::Renderer::GetInstance().SetDimensions(Math::vec2i(width, height));

    Math::mat4f perspective;
    perspective.SetPerspectiveProjection(52.0f, float(width) / float(height), 0.1f, 4000.0f);

    Tools::Logger::WriteInfoLog("Application::OnResize - perspeciveProjectionMatrix: " + to_string(perspective));

    Rendering::Renderer::GetInstance().GetMatrixStack().Multiple(perspective);

    Rendering::Renderer::GetInstance().GetMatrixStack().SetMatrixMode(MODELVIEW_MATRIX);
    Rendering::Renderer::GetInstance().GetMatrixStack().Identity();
}

void Application::SetWindow(AbstractWindow* window) {
    this->window = window;
}

AbstractWindow* Application::GetWindow() {
    return window;
}