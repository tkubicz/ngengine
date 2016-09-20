/* 
 * File:   AbstractCamera.hpp
 * Author: tku
 *
 * Created on 20 September 2016, 17:15
 */

#ifndef ABSTRACTCAMERA_HPP
#define ABSTRACTCAMERA_HPP

#include "NGE/Math/Matrix4.hpp"
#include "NGE/Math/Vector3.hpp"
#include "NGE/Math/Objects3D/Plane.hpp"

namespace NGE {
	namespace Rendering {
		namespace Camera {

			class AbstractCamera {
			  public:
				AbstractCamera();
				virtual ~AbstractCamera();
				
				void SetupProjection(const float fovY, const float aspectRatio, const float near = 0.1f, const float far = 1000.0f);
				
				virtual void Update() = 0;
				virtual void Rotate(const float yaw, const float pitch, const float roll);
				
				const Math::mat4f GetViewMatrix() const;
				const Math::mat4f GetProjectionMatrix() const;
				
				void SetPosition(const Math::vec3f& p);
				const Math::vec3f GetPosition() const;
				
				void SetFOV(const float fov);
				const float GetFOV() const;
				const float GetAspectRatio() const;
				
				void CalculateFrustumPlanes();
				bool IsPointInSphere(const Math::vec3f& point);
				bool IsSphereInFrustum(const Math::vec3f& center, const float radius);
				bool IsBoxInFrustum(const Math::vec3f& min, const Math::vec3f& max);
				void GetFrustumPlanes(Math::vec4f planes[6]);
				
				Math::vec3f farPoints[4];
				Math::vec3f nearPoitns[4];
				
			  protected:
				float yaw, pitch, roll, fov, aspectRatio, zNear, zFar;
				static Math::vec3f UP;
				
				Math::vec3f look;
				Math::vec3f up;
				Math::vec3f right;
				Math::vec3f position;
				Math::mat4f viewMatrix;
				Math::mat4f projectionMatrix;
				Math::Objects3D::planef planes[6];
			};
		}
	}
}

#endif /* ABSTRACTCAMERA_HPP */

