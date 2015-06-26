/* 
 * File:   ParticleForceGenerator.hpp
 * Author: tku
 *
 * Created on 25 maj 2012, 15:54
 */

#ifndef PARTICLEFORCEGENERATOR_HPP
#define	PARTICLEFORCEGENERATOR_HPP

#include <vector>

#include "NGE/Physics/MassAggregate/Particle.hpp"

namespace NGE {
	namespace Physics {
		namespace MassAggregate {

			class ParticleForceGenerator {
			  public:
				virtual void UpdateForce(Particle* particle, float duration) = 0;
			};

			class ParticleGravity : public ParticleForceGenerator {
			  private:
				Math::vec3f gravity;

			  public:
				ParticleGravity(const Math::vec3f& gravity);
				virtual void UpdateForce(Particle* particle, float duration);
			};

			class ParticleDrag : public ParticleForceGenerator {
			  private:
				// Przechowuje wskaźnik prędkości tarcia
				float k1;

				// Przechowuje kwadrat wskaźnika prędkości tarcia
				float k2;

			  public:
				ParticleDrag(float k1, float k2);
				virtual void UpdateForce(Particle* particle, float duration);
			};

			class ParticleAnchoredSpring : public ParticleForceGenerator {
			  protected:
				Math::vec3f* anchor;
				float springConstant;
				float restLength;

			  public:
				ParticleAnchoredSpring();
				ParticleAnchoredSpring(Math::vec3f* anchor, float springConstant, float restLength);

				const Math::vec3f* GetAnchor() const {
					return anchor;
				}
				void Init(Math::vec3f* anchor, float springConstant, float restLength);
				virtual void UpdateForce(Particle* particle, float duration);
			};

			class ParticleAnchoredBungee : public ParticleAnchoredSpring {
			  public:
				virtual void UpdateForce(Particle* particle, float duration);
			};

			class ParticleFakeSpring : public ParticleForceGenerator {
			  private:
				Math::vec3f *anchor;

				float springConstant;

				float damping;
			  public:
				ParticleFakeSpring(Math::vec3f* anchor, float springConstant, float damping);
				virtual void UpdateForce(Particle* particle, float duration);
			};

			class ParticleSpring : public ParticleForceGenerator {
			  private:
				Particle *other;
				float springConstant;
				float restLength;

			  public:
				ParticleSpring(Particle* other, float springConstant, float restLength);
				virtual void UpdateForce(Particle* particle, float duration);
			};

			class ParticleBungee : public ParticleForceGenerator {
			  private:
				Particle* other;
				float springConstant;
				float restLength;

			  public:
				ParticleBungee(Particle* other, float springConstant, float restLength);
				virtual void UpdateForce(Particle* particle, float duration);
			};

			class ParticleBuoyancy : public ParticleForceGenerator {
			  private:
				float maxDepth;
				float volume;
				float waterHeight;
				float liquidDensity;

			  public:
				ParticleBuoyancy(float maxDepth, float volume, float waterHeight, float liquidDensity = 1000.0f);
				virtual void UpdateForce(Particle* particle, float duration);
			};

			class ParticleForceRegistry {
			  protected:

				struct ParticleForceRegistration {
					Particle* particle;
					ParticleForceGenerator* fg;
				};

				typedef std::vector<ParticleForceRegistration> Registry;
				Registry registrations;

			  public:
				void Add(Particle* particle, ParticleForceGenerator* fg);
				void Remove(Particle* particle, ParticleForceGenerator* fg);
				void Clear();
				void UpdateForces(float duration);
			};
		}
	}
}
#endif	/* PARTICLEFORCEGENERATOR_HPP */

