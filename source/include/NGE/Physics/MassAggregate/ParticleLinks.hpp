/* 
 * File:   ParticleLinks.hpp
 * Author: tku
 *
 * Created on 15 czerwiec 2012, 02:27
 */

#ifndef PARTICLELINKS_HPP
#define	PARTICLELINKS_HPP

#include "NGE/Physics/MassAggregate/ParticleContacts.hpp"

namespace NGE
{
    namespace Physics
    {
        namespace MassAggregate
        {

            class ParticleLink : public ParticleContactGenerator
            {
              protected:
                /**
                 * Pobranie aktualnej długości połączenia.
                 * @return Długość połączenia.
                 */
                float currentLength() const;

              public:
                /**
                 * Para cząsteczek które są ze sobą połączone
                 */
                Particle* particle[2];

                /**
                 * Generowanie kontaktu, tak by nie przekroczyło połączenia. Ta klasa może utworzyć tylko pojedynczy
                 * kontakt, więc wskaznik powinien być wskaznikiem do jednego elementu.
                 * @param contact Wskaznik do elementu.
                 * @param limit Limit powinien przyjmować wartość >= 1 (0 jest nieprawidłowe).
                 * @return 0 jeżeli połączenie nie zostało przekroczone, lub 1 jeżeli kontakt wystąpił.
                 */
                virtual unsigned AddContact(ParticleContact* contact, unsigned limit) const = 0;
            };

            class ParticleCable : public ParticleLink
            {
              public:
                /**
                 * Maksymalna długość kabla.
                 */
                float maxLength;

                /**
                 * Sprężystość kabla
                 */
                float restitution;

                /**
                 * Uzupełnienie podanej struktury kontaktu danymi potrzebnymi by kabel nie został zbytnio rozciągnięty.
                 * @param contact Wskaznik do struktury.
                 * @param limit Limit.
                 * @return 
                 */
                virtual unsigned AddContact(ParticleContact* contact, unsigned limit) const;
            };

            /**
             * Połączenie dwóch cząsteczek które generuje kontakt w momencie gdy są zbyt daleko, lub zbyt blisko siebie.
             */
            class ParticleRod : public ParticleLink
            {
              public:
                /**
                 * Długość prętu.
                 */
                float length;

                /**
                 * Uzupełnienie podaje struktury kontaktu danymi potrzebnymi by pręt nie został zbyt rozciągnięty, lub zbliżony.
                 * @param contact Wskaznik do struktury.
                 * @param limit Limit.
                 * @return 
                 */
                virtual unsigned AddContact(ParticleContact* contact, unsigned limit) const;
            };

            /**
             * Ograniczenia (constraints) są jak połączenia (links) tylko, że łączą cząsteczke z punktem który jest stale
             * zaczepiony w przestrzeni (immovable anchor point).
             */
            class ParticleConstraint : public ParticleContactGenerator
            {
              public:
                /**
                 * Cząsteczka połączona z punktem.
                 */
                Particle* particle;

                /**
                 * Punkt z którym połączona jest cząsteczka.
                 */
                Math::vec3f anchor;

              protected:
                /**
                 * Aktualna długość połączenia
                 */
                float currentLength() const;

              public:
                /**
                 * Generowanie kontaktu, tak by nie przekroczyło połączenia. Ta klasa może utworzyć tylko pojedynczy
                 * kontakt, więc wskaznik powinien być wskaznikiem do jednego elementu.
                 * @param contact Wskaznik do elementu.
                 * @param limit Limit powinien przyjmować wartość >= 1 (0 jest nieprawidłowe).
                 * @return 0 jeżeli połączenie nie zostało przekroczone, lub 1 jeżeli kontakt wystąpił.
                 */
                virtual unsigned AddContact(ParticleContact* contact, unsigned limit) const = 0;
            };

            class ParticleCableConstraint : public ParticleConstraint
            {
              public:
                float maxLength;
                float restitution;

                virtual unsigned AddContact(ParticleContact* contact, unsigned limit) const;
            };

            class ParticleRodConstraint : public ParticleConstraint
            {
              public:
                float length;

                virtual unsigned AddContact(ParticleContact* contact, unsigned limit) const;
            };
        }
    }
}
#endif	/* PARTICLELINKS_HPP */

