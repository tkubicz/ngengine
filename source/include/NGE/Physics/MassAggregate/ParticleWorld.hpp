/* 
 * File:   ParticleWorld.hpp
 * Author: tku
 *
 * Created on 21 czerwiec 2012, 02:53
 */

#ifndef PARTICLEWORLD_HPP
#define	PARTICLEWORLD_HPP

#include <vector>

#include "NGE/Physics/MassAggregate/ParticleForceGenerator.hpp"
#include "NGE/Physics/MassAggregate/ParticleLinks.hpp"
#include "NGE/Physics/MassAggregate/ParticleContacts.hpp"

namespace NGE {
	namespace Physics {
		namespace MassAggregate {

			class ParticleWorld {
			  public:
				typedef std::vector<Particle*> Particles;
				typedef std::vector<ParticleContactGenerator*> ContactGenerators;

			  protected:
				/**
				 * Zmienna przechowująca cząsteczki
				 */
				Particles particles;

				bool calculateIterations;
				ParticleForceRegistry registry;
				ParticleContactResolver resolver;
				ContactGenerators contactGenerators;
				ParticleContact* contacts;
				unsigned maxContacts;

			  public:
				ParticleWorld(unsigned maxContacts, unsigned iterations = 0);
				~ParticleWorld();

				unsigned GenerateContacts();
				void Integrate(float duration);
				void RunPhysics(float duration);

				void StartFrame();

				Particles& GetParticles();
				ContactGenerators& GetContactGenerators();
				ParticleForceRegistry& GetForceRegistry();
			};

			class GroundContacts : public ParticleContactGenerator {
				ParticleWorld::Particles* particles;

			  public:
				void Init(ParticleWorld::Particles* particles);

				virtual unsigned AddContact(ParticleContact* contact, unsigned limit) const;
			};
		}
	}
}
#endif	/* PARTICLEWORLD_HPP */

