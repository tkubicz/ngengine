/* 
 * File:   FreeCamera.hpp
 * Author: tku
 *
 * Created on 23 September 2016, 18:03
 */

#ifndef FREECAMERA_HPP
#define FREECAMERA_HPP

#include "NGE/Rendering/Camera/AbstractCamera.hpp"
#include "NGE/Math/Vector3.hpp"

namespace NGE {
	namespace Rendering {
		namespace Camera {
			class FreeCamera : public AbstractCamera {
			  public:
				FreeCamera();
				virtual ~FreeCamera();
				
				void Update() override; 
				void Rotate(const float yaw, const float pitch, const float roll) override;
				
				void Walk(const float amount);
				void Strafe(const float amount);
				void Lift(const float amount);
				
			  protected:
				float yaw, pitch, roll;
				Math::vec3f translation;
			};
		}
	}
}

#endif /* FREECAMERA_HPP */

