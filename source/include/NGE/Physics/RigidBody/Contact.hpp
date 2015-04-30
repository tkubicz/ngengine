/* 
 * File:   Contact.hpp
 * Author: tku
 *
 * Created on 13 styczeń 2014, 14:30
 */

#ifndef CONTACT_HPP
#define	CONTACT_HPP

#include "NGE/Physics/RigidBody/RigidBody.hpp"


namespace NGE
{
    namespace Physics
    {
        namespace RigidBody
        {
            class ContactResolver;

            /**
             * Klasa kontakt reprezentuje dwa obiekty w kontakcie ze sobą. Rozwiązanie
             * kontaktu usuwa ich interpretację i zastosowuje odpowiedni impuls by
             * trzymać je osobno. Zderzające się ciała mogą również odbijać się od
             * siebie. Kontakty mogą również być używane do reprezentacji połączeń,
             * przez zrobienie ich stałymi mogą utrzymywać prawidłową orientację
             * obiektów.
             *
             * Kontakt nie posiada wywoływalnych funkcji, po prostu przechowuje
             * informację o kontakcie. W celu rozwiązania zbioru kontaktów, należy
             * użyć klasy ContactResolver.
             */
            class Contact
            {
              private:
                /**
                 * Klasa ContactResolver potrzebuje dostępu do klasy Contact by móc
                 * rozwiązywać kontakty.
                 */
                friend class ContactResolver;

              public:
                /**
                 * Wskaźniki na obiekty które są ze sobą w kontakcie. Drugi z nich
                 * może mieć wartość NULL, dla kontaktów np. ze scenerią.
                 */
                RigidBody* body[2];
                
                /**
                 * Współczynnik tarcia przy kontakcie.
                 */
                float friction;
                
                /**
                 * Współczynnik restytucji przy kontakcie.
                 */
                float restitution;
                
                /**
                 * Pozycja kontaktu w przestrzeni świata.
                 */
                Math::vec3f contactPoint;
                
                /**
                 * Kierunek kontaktu w przestrzeni świata.
                 */
                Math::vec3f contactNormal;
                
                /**
                 * TODO: Komentarz
                 * Głębokość penetracji w punkcie kontaktu. Jeżeli oba obiekty
                 * są zdefiniowane, wtedy punkt kontaktu powinien być.
                 */
                float penetration;
                
                /**
                 * TODO: Komentarz
                 */
                void SetBodyData(RigidBody* one, RigidBody* two, float friction, float restitution);
                
              protected:
                /**
                 * Macierz transformacji konwertująca współrzędne z przestrzeni
                 * kontaktu do przestrzeni świata. Kolumny tej macierzy tworzą
                 * ortonolmany zbiór wektorów.
                 */
                Math::mat3f contactToWorld;
                
                /**
                 * Prędkość zbliżania się w punkcie kontaktu. To pole jest
                 * ustawiane w momencie wywołania metody CalculateInternals.
                 */
                Math::vec3f contactVelocity;
                
                /**
                 * Wymagana zmiana prędkości aby ten kontakt mógł zostać rozwiązany.
                 */
                float desiredDeltaVelocity;
                
                /**
                 * Pozycja kontaktu względem środka każdego z obiektów w przestrzeni
                 * świata. Ustawiana przez metodę CalculateInternals.
                 */
                Math::vec3f relativeContactPosition[2];
                
                /**
                 * Metoda obliczająca wewnętrzne dane z danych stanu. Wywoływana
                 * zanim algorytm rozwiązywania kontaktów spróbuje rozwiązać
                 * jakikolwiek kontakt. Nie powinna być wywoływana ręcznie.
                 *
                 * @param duration Delta czasu.
                 */
                void CalculateInternals(float duration);
                
                /**
                 * Odwrócenie kontaktu. Metoda zamieniająca ze sobą dwa ciała
                 * sztywne i odwracająca normalną kontaktu. Wartości stanu po
                 * zamianie powinny być przeliczone za pomocą CalculateInternals
                 * (nie dzieje się to automatycznie).
                 */
                void SwapBodies();
                
                /**
                 * Aktualizacja stanu uśpienia obiektów które biorą udział w
                 * kontakcie. Obiekt przestanie być uśpiony jeżeli jest w kontakcie
                 * z obiektem, który nie jest uśpiony.
                 */
                void MatchAwakeState();
                
                /**
                 * Wyliczenie i ustawienie wymaganej zmiany prędkości.
                 *
                 * @param duration Delta czasu.
                 */
                void CalculateDesiredDeltaVelocity(float duration);
                
                /**
                 * TODO: Komentarz
                 */
                Math::vec3f CalculateLocalVelocity(unsigned bodyIndex, float duration);
                
                /**
                 * TODO: Komentarz
                 */
                void CalculateContactBasis();
                
                /**
                 * Zastosonie impulsu do podanego ciała zwracając zmianę w prędkościach.
                 * TODO: Uzupełnić komentarz
                 */
                void ApplyImpulse(const Math::vec3f& impulse, RigidBody* body, Math::vec3f* velocityChange, Math::vec3f* rotationChange);
                
                /**
                 * TODO: Komentarz
                 */
                void ApplyVelocityChange(Math::vec3f velocityChange[2], Math::vec3f rotationChange[2]);
                
                /**
                 * TODO: Komentarz
                 */
                void ApplyPositionChange(Math::vec3f linearChange[2], Math::vec3f angularChange[2], float penetration);
                
                /**
                 * TODO: Komentarz
                 */
                Math::vec3f CalculateFricionlessImpulse(Math::mat3f* inverseInertiaTensor);
                
                /**
                 * TODO: Komentarz
                 */
                Math::vec3f CalculateFrictionImpulse(Math::mat3f* inverseInertiaTensor);
            };
        }
    }
}

#endif	/* CONTACT_HPP */

