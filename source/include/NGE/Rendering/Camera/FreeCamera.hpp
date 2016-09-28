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

				void Walk(const float dt);
				void Strafe(const float dt);
				void Lift(const float dt);

				float GetSpeed() const;
				void SetSpeed(float speed);

				Math::vec3f GetTranslation() const;
				void SetTranslation(Math::vec3f translation);

			  protected:

				/**
				 * Move speed of the camera in m/s.
				 */
				float speed;

				Math::vec3f translation;
			};
		}
	}
}

#endif /* FREECAMERA_HPP */

