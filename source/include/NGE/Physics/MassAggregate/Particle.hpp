/* 
 * File:   Particle.hpp
 * Author: tku
 *
 * Created on 13 kwiecień 2012, 13:59
 */

#ifndef PARTICLE_HPP
#define	PARTICLE_HPP

#include "NGE/Math/Vector3.hpp"

namespace NGE {
	namespace Physics {
		namespace MassAggregate {

			class Particle {
			  protected:
				float inverseMass;
				float damping;

				Math::vec3f position;
				Math::vec3f velocity;
				Math::vec3f acceleration;
				Math::vec3f forceAccum;

			  public:
				void Integrate(float duration);

				void SetMass(const float mass);
				float GetMass() const;

				void SetInverseMass(const float inverseMass);
				float GetInverseMass() const;

				bool HasFiniteMass() const;

				void SetDamping(const float damping);
				float GetDamping() const;

				void SetPosition(const Math::vec3f &position);
				void SetPosition(const float x, const float y, const float z);
				void GetPosition(Math::vec3f* position) const;
				Math::vec3f GetPosition() const;

				void SetVelocity(const Math::vec3f& velocity);
				void SetVelocity(const float x, const float y, const float z);
				void GetVelocity(Math::vec3f* velocity) const;
				Math::vec3f GetVelocity() const;

				void SetAcceleration(const Math::vec3f& acceleration);
				void SetAcceleration(const float x, const float y, const float z);
				void GetAcceleration(Math::vec3f* acceleration) const;
				Math::vec3f GetAcceleration() const;

				void ClearAccumulator();
				void AddForce(const Math::vec3f& force);
			};
		}
	}
}

#endif	/* PARTICLE_HPP */

