/* 
 * File:   CollisionBox.hpp
 * Author: tku
 *
 * Created on 15 styczeń 2014, 13:25
 */

#ifndef COLLISIONBOX_HPP
#define	COLLISIONBOX_HPP

#include "NGE/Physics/RigidBody/CollisionPrimitive.hpp"
#include "NGE/Math/Vector3.hpp"

namespace NGE
{
    namespace Physics
    {
        namespace RigidBody
        {

            /**
             * Reprezentacja ciała sztywnego które może być traktowane
             * jako pudełko do wykrywania kolizjii.
             */
            class CollisionBox : public CollisionPrimitive
            {
              public:
                /**
                 * Połowa wielkości pudełka.
                 */
                Math::vec3f halfSize;
            };
        }
    }
}

#endif	/* COLLISIONBOX_P */