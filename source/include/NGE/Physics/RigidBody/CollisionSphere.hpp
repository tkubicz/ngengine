/* 
 * File:   CollisionSphere.hpp
 * Author: tku
 *
 * Created on 15 styczeń 2014, 13:24
 */

#ifndef COLLISIONSPHERE_HPP
#define	COLLISIONSPHERE_HPP

#include "NGE/Physics/RigidBody/CollisionPrimitive.hpp"


namespace NGE
{
    namespace Physics
    {
        namespace RigidBody
        {

            /**
             * Reprezentacja ciała sztywnego które może być traktowane
             * jako kula do wykrywania kolizjii.
             */
            class CollisionSphere : public CollisionPrimitive
            {
              public:
                /**
                 * Promień kuli
                 */
                float radius;
            };
        }
    }
}

#endif	/* COLLISIONSPHERE_HPP */

