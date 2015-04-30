/* 
 * File:   Camera.hpp
 * Author: tku
 *
 * Created on 6 czerwiec 2012, 16:42
 */

#ifndef CAMERA_HPP
#define	CAMERA_HPP

#include "NGE/Math/Vector2.hpp"
#include "NGE/Math/Vector3.hpp"
#include "NGE/Math/Matrix4.hpp"
#include "NGE/Events/KeyboardEvent.hpp"
#include "NGE/Rendering/Camera/Frustum.hpp"

namespace NGE
{
    namespace Rendering
    {
        namespace Camera
        {

            class Camera
            {
              public:
                Camera(float positionX = 0.0f, float positionY = 0.0f, float positionZ = 0.0f,
                       float focusX = 0.0f, float focusY = 0.0f, float focusZ = 0.0f,
                       float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f);

                void Set(const Math::vec3f& position, const Math::vec3f& focus, const Math::vec3f& up);
                void Set(float positionX, float positionY, float positionZ,
                         float focusX, float focusY, float focusZ,
                         float upX, float upY, float upZ);

                void Update(float dt);
                void Look();

                //Frustum& GetFrustum();

                void LockMouse(bool lock);
                bool IsMouseLocked();

                void SetKeyboardInput(Events::KeyboardEvent& event);
                void SetMouseInfo(const Math::vec2i& mousePosition);
                void SetMouseInfo(int x = 0, int y = 0);

                const Math::vec3f& GetViewerPosition() const;
                const Math::vec3f& GetFocusPosition() const;
                const Math::vec3f& GetUpDirection() const;

                void RotateAroundPoint(const Math::vec3f& center, float angle, float x, float y, float z);

              private:
                void ProcessKeyboardEvents();

                void Move(float speed);
                void StrafeMove(float speed);
                void RotateView(float angle, float x, float y, float z);
                void SetViewByMouse();

                //Frustum frustum;
                Math::vec2i mouseLockedPosition, mousePosition;
                Math::vec3f position, strafe, focus, up;
                Math::mat4f viewMatrix;
                bool activeKeys[6], mouseLocked;
                float speed, translationSpeed, mouseSensitivity, mouseSpeed, currentXRotation;
            };
        }
    }
}
#endif	/* CAMERA_HPP */

