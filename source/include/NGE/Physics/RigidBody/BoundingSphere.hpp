/* 
 * File:   BoundingSphere.hpp
 * Author: tku
 *
 * Created on 27 styczeń 2014, 13:29
 */

#ifndef BOUNDINGSPHERE_HPP
#define	BOUNDINGSPHERE_HPP

#include "NGE/Math/Vector3.hpp"

namespace NGE
{
    namespace Physics
    {
        namespace RigidBody
        {
            /**
             * Klasa reprezentująca sferę otaczającą obiekt, która może być
             * testowana na zdarzenie nakładania się na siebie z inną sferą.
             */
            class BoundingSphere
            {
              protected:
                Math::vec3f center;
                float radius;
                
              public:
                /**
                 * Utworzenie nowej sfery.
                 *
                 * @param center Środek sfery.
                 * @param radius Promień sfery.
                 */
                BoundingSphere(const Math::vec3f& center, float radius);
                
                /**
                 * Utworzenie nowej sfery otaczającej dwie podane w parametrach.
                 *
                 * @param one Pierwsza sfera.
                 * @param two Druga sfera.
                 */
                BoundingSphere(const BoundingSphere& one, const BoundingSphere& two);
                
                /**
                 * Sprawdzenie czy sfera nakłada się z drugą sferą.
                 *
                 * @param other Sfera do przetestowania nakładania się.
                 */
                bool Overlaps(const BoundingSphere* other) const;
                
                /**
                 * TODO: Komentarz
                 */
                float GetGrowth(const BoundingSphere& other) const;
                
                float GetSize() const;
            };
        }
    }
}

#endif	/* BOUNDINGSPHERE_HPP */

