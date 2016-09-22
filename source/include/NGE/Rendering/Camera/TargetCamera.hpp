/* 
 * File:   TargetCamera.hpp
 * Author: tku
 *
 * Created on 22 September 2016, 17:40
 */

#ifndef TARGETCAMERA_HPP
#define TARGETCAMERA_HPP

#include "NGE/Rendering/Camera/AbstractCamera.hpp"
#include "NGE/Math/Vector3.hpp"

namespace NGE {
	namespace Rendering {
		namespace Camera {

			class TargetCamera : public AbstractCamera {
			  public:
				TargetCamera();
				virtual ~TargetCamera();

				void Update() override;
				void Rotate(const float yaw, const float pitch, const float roll) override;

				void SetTarget(const Math::vec3f& tgt);
				const Math::vec3f GetTarget() const;

				void Pan(const float dx, const float dy);
				void Zoom(const float amount);
				void Move(const float dx, const float dz);

			  protected:
				Math::vec3f target;
				float minRy, maxRy;
				float distance;
				float minDistance, maxDistance;
			};
		}
	}
}

#endif /* TARGETCAMERA_HPP */

