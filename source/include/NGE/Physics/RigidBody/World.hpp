/* 
 * File:   World.hpp
 * Author: tku
 *
 * Created on 10 luty 2014, 02:53
 */

#ifndef WORLD_HPP
#define	WORLD_HPP

#include "NGE/Physics/RigidBody/ContactResolver.hpp"
#include "NGE/Physics/RigidBody/ContactGenerator.hpp"


namespace NGE {
	namespace Physics {
		namespace RigidBody {

			/**
			 * The world class represents an independent simulation of physics.
			 * It keeps track of set of rigid bodies, and provide the means to
			 * update them all.
			 */
			class World {
			  protected:
				/**
				 * True if the world should calculate the number of iterations
				 * to give the contact resolver at each frame.
				 */
				bool calculateIterations;

				/**
				 * Holds a single rigid body in linked list of bodies.
				 */
				struct BodyRegistration {
					RigidBody* body;
					BodyRegistration* next;
				};

				/**
				 * Holds the head of the list of registered bodies.
				 */
				BodyRegistration* firstBody;

				/**
				 * Holds the resolver for sets of contacts.
				 */
				ContactResolver resolver;

				/**
				 * Holds one contact generators in a linked list.
				 */
				struct ContactGenRegistration {
					ContactGenerator* gen;
					ContactGenRegistration* next;
				};

				/**
				 * Holds the head of the list of contact generators.
				 */
				ContactGenRegistration* firstContactGen;

				/**
				 * Holds an array of contacts, for filling by the contact generators.
				 */
				Contact* contacts;

				/**
				 * Holds the maximum number of contacts allowed (i.e. the size
				 * of the contacts array).
				 */
				unsigned maxContacts;

			  public:

				/**
				 * Creates a new simulatior that can handle up to the given number
				 * of contacts per frame. You can also optionally give a number
				 * of contact-resolution iterations to use. If you don't give
				 * a number of iterations, then four times the number of detected
				 * contacts will be used for each frame.
				 * 
				 * @param maxContacts Maximum contacts per frame.
				 * @param iterations Contact-resolution iterations to use.
				 */
				World(unsigned maxContacts, unsigned iterations = 0);

				/**
				 * Default destructor.
				 */
				virtual ~World();

				/**
				 * Calls each of the registered contact generators to report their
				 * contacts.
				 * 
				 * @return The number of generated contacts.
				 */
				unsigned GenerateContacts();

				/**
				 * Processes all the physics for the world.
				 * 
				 * @param duration Delta time between frames.
				 */
				void RunPhysics(float duration);

				/**
				 * Initialises the world for a simulation frame. This clears the
				 * force and the torque accumulators for bodies in the world.
				 * After calling this, the bodies an have their forces and torques
				 * for this frame added.
				 */
				void StartFrame();
			};
		}
	}
}

#endif	/* WORLD_HPP */

