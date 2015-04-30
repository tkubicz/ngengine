#ifdef ANDROID
#include "NGE/Windows/AndroidWindow.hpp"

using namespace NGE::Windows;

AndroidWindow::AndroidWindow() {
    isRunning = true;
}

bool AndroidWindow::Create() {
}

bool AndroidWindow::Create(int width, int height, int bpp, bool fullscreen, const std::string& title, int fsaa, int openglMajor, int openglMinor) {
}

bool AndroidWindow::CreateSlave(bool visible) {
    return false;
}

bool AndroidWindow::Init() {
    return true;
}

void AndroidWindow::Destroy() {
}

void AndroidWindow::SetMainWindowContext() {
}

void AndroidWindow::SetSlaveWindowContext() {
}

void AndroidWindow::ProcessEvents() {
}

void AndroidWindow::SetInputCallbacks() {
}

bool AndroidWindow::LoadXMLSettings(pugi::xml_node& windowNode) {
    return true;
}

bool AndroidWindow::IsRunning() {
}

void AndroidWindow::SwapBuffers() {
}

void AndroidWindow::SetWindowTitle(const std::string& title) {
}

std::string AndroidWindow::GetWindowTitle() const {
    return "window title";
}

int AndroidWindow::GetWidth() const {
    return 1;
}

int AndroidWindow::GetHeight() const {
    return 1;
}

NGE::Math::vec2i AndroidWindow::GetSize() const {
    return Math::vec2i(1, 1);
}

void AndroidWindow::SetWidth(const int width) {
}

void AndroidWindow::SetHeight(const int height) {
}

void AndroidWindow::SetSize(const Math::vec2i& size) {
}

int AndroidWindow::GetXPosition() const {
}

int AndroidWindow::GetYPosition() const {
}

NGE::Math::vec2i AndroidWindow::GetPosition() const {
    return Math::vec2i(1, 1);
}

void AndroidWindow::SetPosition(const Math::vec2i& position) {
}

bool AndroidWindow::GetVSync() {
    return true;
}

void AndroidWindow::SetVSync(bool vsync) {
}

void AndroidWindow::EnableMouseCursor(bool) {
}

bool AndroidWindow::IsMouseCursorEnable() {
    return false;
}

NGE::Math::vec2i AndroidWindow::GetMousePosition() const {
    return Math::vec2i(0, 0);
}

#endif