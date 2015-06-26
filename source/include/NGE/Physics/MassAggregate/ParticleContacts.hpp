/* 
 * File:   ParticleContact.hpp
 * Author: tku
 *
 * Created on 14 czerwiec 2012, 01:18
 */

#ifndef PARTICLECONTACTS_HPP
#define	PARTICLECONTACTS_HPP

#include "NGE/Physics/MassAggregate/Particle.hpp"

namespace NGE {
	namespace Physics {
		namespace MassAggregate {
			class ParticleContactResolver;

			class ParticleContact {
				friend class ParticleContactResolver;

			  public:
				Particle* particle[2];
				float restitution;
				Math::vec3f contactNormal;
				float penetration;
				Math::vec3f particleMovment[2];

			  protected:
				void Resolve(float duration);
				float CalculateSeperatingVelocity() const;

			  private:
				void ResolveVelocity(float duration);
				void ResolveInterpenetration(float duration);
			};

			class ParticleContactResolver {
			  protected:
				unsigned iterations;
				unsigned iterationsUsed;

			  public:
				ParticleContactResolver(unsigned iterations);

				void SetIterations(unsigned iterations);
				void ResolveContacts(ParticleContact* contactArray, unsigned numContacts, float duration);
			};

			class ParticleContactGenerator {
			  public:
				virtual unsigned AddContact(ParticleContact* contact, unsigned limit) const = 0;
			};
		}
	}
}

#endif	/* PARTICLECONTACT_HPP */

