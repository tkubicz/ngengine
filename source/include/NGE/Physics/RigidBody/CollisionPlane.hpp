/* 
 * File:   CollisionPlane.hpp
 * Author: tku
 *
 * Created on 15 styczeń 2014, 13:24
 */

#ifndef COLLISIONPLANE_HPP
#define	COLLISIONPLANE_HPP

namespace NGE
{
    namespace Physics
    {
        namespace RigidBody
        {

            /**
             * Płaszczyzna nie jest primitywem - nie przedstawia ciała
             * sztywnego. Jest używana do kontaktu z "nieporuszalnymi"
             * elementami geometrii świata.
             */
            class CollisionPlane
            {
              public:
                /**
                 * Wektor normalny płaszczyzny.
                 */
                Math::vec3f direction;

                /**
                 * Odległość płaszczyzny od punktu początkowego.
                 */
                float offset;
            };
        }
    }
}

#endif	/* COLLISIONPLANE_HPP */

