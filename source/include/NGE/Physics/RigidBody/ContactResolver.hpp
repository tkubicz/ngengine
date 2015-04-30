/* 
 * File:   ContactResolver.hpp
 * Author: tku
 *
 * Created on 15 styczeń 2014, 16:12
 */

#ifndef CONTACTRESOLVER_HPP
#define	CONTACTRESOLVER_HPP

#include "NGE/Physics/RigidBody/Contact.hpp"

namespace NGE
{
    namespace Physics
    {
        namespace RigidBody
        {
            // TODO: Komentarz
            class ContactResolver
            {
              protected:
                /**
                 * Liczba iteracji do wykonania podczas rozwiązywania prędkości.
                 */
                unsigned velocityIterations;
                
                /**
                 * Liczba iteracji do wykonania podczas rozwiązywania pozycji.
                 */
                unsigned positionIterations;
                
                /**
                 * W celu zapewnienia stabilności prędkości mniejsze niż ta wartość
                 * są uznawane za 0. Zbyt mała wartość może spowodować niestabilność
                 * symulacji, zbyt duża może powodować graficzne artefakty. Dobrą
                 * wartością na początek jest 0.01.
                 */
                float velocityEpsilon;
                
                /**
                 * TODO: Komentarz
                 */
                float positionEpsilon;
                
                void PrepareContacts(Contact* contactArray, unsigned numContacts, float duration);
                
                void AdjustVelocities(Contact* contactArray, unsigned numContacts, float duration);
                
                void AdjustPosition(Contact* contacts, unsigned numContacts, float duration);
                
              public:
                /**
                 * Liczba iteracji prędkości użyta podczas ostatniego wywołania
                 * do rozwiązania kontaktów.
                 */
                unsigned velocityIterationsUsed;
                
                /**
                 * Liczba iteracji pozycji użyta podczas ostatniego wywołania
                 * do rozwiązania kontaktów.
                 */
                unsigned positionIterationsUsed;
                
                ContactResolver(unsigned iterations, float velocityEpsilon = 0.01, float positionEpsilon = 0.01);
                
                ContactResolver(unsigned velocityIterations, unsigned positionIterations, float velocityEpsilon = 0.01, float positionEpsilon = 0.01);
                
                bool IsValid();
                
                void SetIterations(unsigned velocityIterations, unsigned positionIterations);
                
                void SetIterations(unsigned iterations);
                
                void SetEpsilon(float velocityEpsilon, float positionEpsilon);
                
                void ResolveContacts(Contact* contactArray, unsigned numContacts, float duration);
                
              private:
                
                bool validSettings;
            };
        }
    }
}

#endif	/* CONTACTRESOLVER_HPP */

