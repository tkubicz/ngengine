#include "NGE/Rendering/Camera/Camera.hpp"
#include "NGE/Rendering/Renderer.hpp"

using namespace NGE::Rendering::Camera;

Camera::Camera(float positionX, float positionY, float positionZ, float focusX, float focusY, float focusZ, float upX, float upY, float upZ) {
    speed = 0.0f;
    translationSpeed = 30.0f;
    mouseSensitivity = 80.0f;
    currentXRotation = 0.0f;

    mouseLocked = false;

    memset(activeKeys, false, 6);
    Set(positionX, positionY, positionZ, focusX, focusY, focusZ, upX, upY, upZ);
}

void Camera::Set(const NGE::Math::vec3f& position, const NGE::Math::vec3f& focus, const NGE::Math::vec3f& up) {
    this->position = position;
    this->focus = focus;
    this->up = up;
}

void Camera::Set(float positionX, float positionY, float positionZ, float focusX, float focusY, float focusZ, float upX, float upY, float upZ) {
    position.Set(positionX, positionY, positionZ);
    focus.Set(focusX, focusY, focusZ);
    up.Set(upX, upY, upZ);
}

void Camera::Update(float dt) {
    strafe = (focus - position).CrossProduct(up);
    strafe.Normalize();

    speed = dt * translationSpeed;
    //mouseSpeed = dt * mouseSensitivity;
    mouseSpeed = 1.0f - exp(log(0.5f)) * mouseSensitivity * dt;

    SetViewByMouse();
    ProcessKeyboardEvents();

    viewMatrix.LookAt(position, focus, up);

    //frustum.Update(viewMatrix, Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));
}

void Camera::Look() {
    Renderer::GetInstance().GetMatrixStack().SetMatrixMode(MODELVIEW_MATRIX);
    Renderer::GetInstance().GetMatrixStack().LoadMatrix(viewMatrix);
}

//NGE::Rendering::Camera::Frustum& Camera::GetFrustum()
//{
//    return frustum;
//}

void Camera::SetTouchInput(Events::TouchEvent& event) {
    int y = event.getYPosition();
    int x = event.getXPosition();
    log_info("SetTouchInput: x=" + to_string(x) + ", y=" + to_string(y));
    if (y < 900 && x > 270 && x < 810) {
        activeKeys[0] = (event.getType() == 1 || event.getType() == 3) ? true : false;
    }
}

void Camera::SetKeyboardInput(NGE::Events::KeyboardEvent& event) {
    switch (event.GetKeyId()) {
        case Events::KEY_W:
            activeKeys[0] = event.GetAction() == 1 || event.GetAction() == 2 ? true : false;
            break;

        case Events::KEY_S:
            activeKeys[1] = event.GetAction() == 1 || event.GetAction() == 2 ? true : false;
            break;

        case Events::KEY_A:
            activeKeys[2] = event.GetAction() == 1 || event.GetAction() == 2 ? true : false;
            break;

        case Events::KEY_D:
            activeKeys[3] = event.GetAction() == 1 || event.GetAction() == 2 ? true : false;
            break;
    }
}

void Camera::LockMouse(bool lock) {
    if (lock)
        mouseLockedPosition = mousePosition;

    mouseLocked = lock;
}

bool Camera::IsMouseLocked() {
    return mouseLocked;
}

void Camera::SetMouseInfo(const NGE::Math::vec2i& mousePosition) {
    SetMouseInfo(mousePosition.x, mousePosition.y);
}

void Camera::SetMouseInfo(int x, int y) {
    mousePosition.Set(x, y);
}

const NGE::Math::vec3f& Camera::GetViewerPosition() const {
    return position;
}

const NGE::Math::vec3f& Camera::GetFocusPosition() const {
    return focus;
}

const NGE::Math::vec3f& Camera::GetUpDirection() const {
    return up;
}

void Camera::ProcessKeyboardEvents() {
    if (activeKeys[0]) Move(speed);
    if (activeKeys[1]) Move(-speed);
    if (activeKeys[2]) StrafeMove(-speed);
    if (activeKeys[3]) StrafeMove(speed);
}

void Camera::Move(float speed) {
    Math::vec3f temp = focus - position;
    temp.Normalize();

    temp *= speed;

    position += temp;
    focus += temp;
}

void Camera::StrafeMove(float speed) {
    position.x += strafe.x * speed;
    position.z += strafe.z * speed;

    focus.x += strafe.x * speed;
    focus.z += strafe.z * speed;
}

void Camera::RotateView(float angle, float x, float y, float z) {
    Math::vec3f newView;
    Math::vec3f vView = focus - position;

    float cosTheta = cos(angle);
    float sinTheta = sin(angle);

    newView.x = (cosTheta + (1 - cosTheta) * x * x) * vView.x;
    newView.x += ((1 - cosTheta) * x * y - z * sinTheta) * vView.y;
    newView.x += ((1 - cosTheta) * x * z + y * sinTheta) * vView.z;

    newView.y = ((1 - cosTheta) * x * y + z * sinTheta) * vView.x;
    newView.y += (cosTheta + (1 - cosTheta) * y * y) * vView.y;
    newView.y += ((1 - cosTheta) * y * z - x * sinTheta) * vView.z;

    newView.z = ((1 - cosTheta) * x * z - y * sinTheta) * vView.x;
    newView.z += ((1 - cosTheta) * y * z + x * sinTheta) * vView.y;
    newView.z += (cosTheta + (1 - cosTheta) * z * z) * vView.z;

    focus = position + newView;
}

void Camera::RotateAroundPoint(const NGE::Math::vec3f& center, float angle, float x, float y, float z) {
    Math::vec3f newPosition;
    Math::vec3f pos = position - center;

    float cosTheta = cos(angle);
    float sinTheta = sin(angle);

    newPosition.x = (cosTheta + (1 - cosTheta) * x * x) * pos.x;
    newPosition.x += ((1 - cosTheta) * x * y - z * sinTheta) * pos.y;
    newPosition.x += ((1 - cosTheta) * x * z + y * sinTheta) * pos.z;

    newPosition.y = ((1 - cosTheta) * x * y + z * sinTheta) * pos.x;
    newPosition.y += (cosTheta + (1 - cosTheta) * y * y) * pos.y;
    newPosition.y += ((1 - cosTheta) * y * z - x * sinTheta) * pos.z;

    newPosition.z = ((1 - cosTheta) * x * z - y * sinTheta) * pos.x;
    newPosition.z += ((1 - cosTheta) * y * z + x * sinTheta) * pos.y;
    newPosition.z += (cosTheta + (1 - cosTheta) * z * z) * pos.z;

    position = center + newPosition;
}

void Camera::SetViewByMouse() {
    float angleY, angleZ;
    angleY = angleZ = 0.0f;

    if ((mousePosition == mouseLockedPosition) || !mouseLocked)
        return;

    angleY = Math::MathUtils::DegToRad(float(mouseLockedPosition.x - mousePosition.x) * mouseSpeed);
    angleZ = Math::MathUtils::DegToRad(float(mouseLockedPosition.y - mousePosition.y) * mouseSpeed);

    currentXRotation -= angleZ * mouseSpeed;
    Math::vec3f vAxis = (focus - position).CrossProduct(up);
    vAxis.Normalize();

    RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
    RotateView(angleY, 0, 1, 0);

    if (mouseLocked)
        mouseLockedPosition = mousePosition;
}

NGE::Math::mat4f Camera::getViewMatrix() {
    return viewMatrix;
}
