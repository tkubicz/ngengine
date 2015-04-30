/* 
 * File:   CollisionPrimitive.hpp
 * Author: tku
 *
 * Created on 15 styczeń 2014, 13:11
 */

#ifndef COLLISIONPRIMITIVE_HPP
#define	COLLISIONPRIMITIVE_HPP

#include "NGE/Physics/RigidBody/RigidBody.hpp"
#include "NGE/Math/Matrix4.hpp"
#include "NGE/Math/Vector3.hpp"

namespace NGE
{
    namespace Physics
    {
        namespace RigidBody
        {
            class IntersectionTests;
            class CollisionDetector;

            /**
             * Klasa reprezentująca obiekt do detekcji kolizjii
             */
            class CollisionPrimitive
            {
              public:
                friend class IntersectionTests;
                friend class CollisionDetector;

                /**
                 * Ciało sztywne reprezentowane przez ten prymityw.
                 */
                RigidBody* body;

                /**
                 * Offset prymitywu od ciała sztywnego.
                 */
                Math::mat4f offset;

                void CalculateInternals();

                /**
                 * Metoda dostępu do wektorów osi w macierzy transformacji.
                 */
                Math::vec3f GetAxis(unsigned index) const;

                /**
                 * Zwrócenie macierzy transformacji obliczonej z połączenia
                 * offsetu i transformacji (orientacja + pozycja) ciała sztywnego
                 * które jest przypisane do tego prymitywu.
                 */
                const Math::mat4f& GetTransform() const;

              protected:
                Math::mat4f transform;
            };
        }
    }
}

#endif	/* COLLISIONPRIMITIVE_HPP */

