/* 
 * File:   RigidBody.hpp
 * Author: tku
 *
 * Created on 3 styczeń 2014, 16:48
 */

#ifndef RIGIDBODY_HPP
#define	RIGIDBODY_HPP

#include "NGE/Math/Matrix3.hpp"
#include "NGE/Math/Matrix4.hpp"
#include "NGE/Math/Quaternion.hpp"
#include "NGE/Math/Vector3.hpp"

namespace NGE
{
    namespace Physics
    {
        namespace RigidBody
        {

            /**
             * Ciało sztywne (rigid body) to podstawowy obiekt symulacji fizycznej.
             * 
             * Posiada pozycję, orientację wraz z pierwszymi pochodnymi. Może być
             * całkowany przez czas. Siły, tarcie i impulsy (linowy i kątowy) mogą
             * być na nim zastosowane. Ciało zarządza swoim stanem i umożliwia dostęp
             * przez zestaw metod.
             */
            class RigidBody
            {
              protected:
                static const float sleepEpsilon;

                /**
                 * Pole przechowujące odwróconą masę ciała sztywnego. Przechowywanie
                 * odwróconej masy jest bardziej użyteczne, ponieważ całkowanie jest
                 * prostsze. Taka implementacja umożliwia posiadanie obiektów o
                 * nieskończonej masie (niemożliwe do poruszenia).
                 */
                float inverseMass;

                /**
                 * Pole przechowujące odwrócony tensor bezwładności ciała. Dany tensor
                 * bezwładności nie może być zdegenerowany (tzn. że ciało ma zerową
                 * bezwładność względem obracania się wokół jednej z osi). Tak długo
                 * jak tensor jest skończony, będzie nieodwracalny. Odwrócony tensor
                 * został użyty z podobnych przyczyn jak odwrócona masa.
                 * 
                 * Tensor bezwładności, inaczej niż inne pola zdefiniowane w tej klasie,
                 * jest dany w przestrzeni obiektu.
                 */
                Math::mat3f inverseInertiaTensor;

                /**
                 * Pole przechowujące wartość tłumienia stosowną do ruchu liniowego.
                 * Tłumienie jest potrzebne do usuwania energii która została dodana
                 * podczas błędów numerycznych całkowania.
                 */
                float linearDamping;

                /**
                 * Pole przechowujące wartość tłumienia stosowaną do ruchu kątowego.
                 * Tłumienie jest potrzebne do usuwania energii która została dodana
                 * podczas błędów numerycznych całkowania.
                 */
                float angularDamping;

                /**
                 * Pole przechowujące liniową pozycję ciała w przestrzeni świata.
                 */
                Math::vec3f position;

                /**
                 * Pole przechowujące kątową orientację ciała w przestrzeni świata.
                 */
                Math::quaternionf orientation;

                /**
                 * Pole przechowujące liniową prędkość ciała w przestrzeni świata.
                 */
                Math::vec3f velocity;

                /**
                 * Pole przechowujące kątową prędkość, lub rotację, ciała w przestrzeni świata.
                 */
                Math::vec3f rotation;

                /**
                 * Pole przechowujące tensor bezwładności ciała w przestrzeni świata.
                 * Pole jest zdefiniowane w lokalnej przestrzeni ciała.
                 */
                Math::mat3f inverseInertiaTensorWorld;

                /**
                 * Pole przechowujące wartość ruchu ciała. 
                 */
                float motion;

                /**
                 * Ciało może zostać uśpione w celu uniknięcia aktualizacji
                 * przez funkcję całkującą lub wykrywanie kolizji ze światem.
                 */
                bool isAwake;

                /**
                 * Niektóre ciała nigdy nie powinny zasypiać np. kontrolowane
                 * przez gracza postacie.
                 */
                bool canSleep;

                /**
                 * Pole przechowujące macierz transformacji używaną w celu konwersji
                 * z przestrzeni ciała na przestrzeń świata i odwrotnie. W tym celu
                 * należy użyć funkcji GetPointIn*Space.
                 * 
                 * @see GetPointInLocalSpace
                 * @see GetPointInWorldSpace
                 * @see GetTransform
                 */
                Math::mat4f transformMatrix;

                /**
                 * @name Akumulatory siły i tarcia
                 * 
                 * Te pola przechowują aktualną siłę, tarcie i przyśpieszenie ciała sztywnego.
                 * Siły mogą być dodane do obiektu w dowolnej kolejności i klasa rozkłada je
                 * na części składowe, kumulując je do następnego korku symulacji. W momencie
                 * symulacji, przyśpieszenie jest wyliczane i przypisywane obiektowi.
                 */
                /*@{*/

                /**
                 * Pole przechowujące skumulowaną siłę która zostanie zastosowana w następnym
                 * kroku całkowania.
                 */
                Math::vec3f forceAccum;

                /**
                 * Pole przechowujące skumulowane tarcie które zostanie zastosowane w następnym
                 * kroku całkowania.
                 */
                Math::vec3f torqueAccum;

                /**
                 * Pole przechowujące przyśpieszenie ciała sztywnego. Ta wartość może zostać
                 * użyta do ustawienia przyśpieszenia względem grawitacji (główne użycie), albo
                 * innego stałego przyśpieszenia.
                 */
                Math::vec3f acceleration;

                /**
                 * Liniowe przyśpieszenie ciała sztywnego dla poprzedniej klatki.
                 */
                Math::vec3f lastFrameAcceleration;

                /*@}*/

                /**
                 * @name Funkcje pomocniczne
                 */
                /*@{*/

                /**
                 * Transformacja tensora bezwładności przez kwaternion.
                 * @param iitWorld Zwracany odwrócony tensor bezwładności w przestrzeni świata.
                 * @param q Dany kwaternion orientacji.
                 * @param iitBody Dany tensor bezwładności w lokalnej przestrzeni ciała.
                 * @param rotmat Macierz rotacji.
                 */
                static inline void TransformInertiaTensor(Math::mat3f& iitWorld, const Math::quaternionf& q, const Math::mat3f& iitBody, const Math::mat4f& rotmat);

                /**
                 * Metoda tworząca macierz transformacji z pozycji i orientacji.
                 * @param transformMatrix Zwracana macierz transformacji.
                 * @param position Dana pozycja ciała. 
                 * @param orientation Dana orientacja ciała.
                 */
                static inline void CalculateTransformMatrix(Math::mat4f& transformMatrix, const Math::vec3f& position, const Math::quaternionf& orientation);

                /*@}*/

              public:
                /**
                 * @name Funkcje symulacji i całkowania
                 * 
                 * Funkcje używane do symulowania ruchu ciała sztywnego w czasie.
                 * W normalnej aplikacji ustawia jedno lub wiele ciał sztywnych, 
                 * nadaje siły ciągłe (jak grawitacja), następnie dodaje siły w
                 * każdej klatce, całkuje, przed wyświetleniem.
                 * 
                 * Aktualnie jedyną funkcją całkującą jest metoda Newtona Eulera.
                 */
                /*@{*/

                /**
                 * 
                 */
                void CalculateDerivedData();

                void Integrate(float duration);

                /*@}*/

                void SetMass(const float mass);

                float GetMass() const;

                void SetInverseMass(const float inverseMass);

                float GetInverseMass() const;

                bool HasFiniteMass() const;

                void SetInertiaTensor(const Math::mat3f& inertiaTensor);

                void GetInertiaTensor(Math::mat3f* inertiaTensor) const;

                Math::mat3f GetInertiaTensor() const;

                void GetInertiaTensorWorld(Math::mat3f* inertiaTensor) const;

                Math::mat3f GetInertiaTensorWorld() const;

                void SetInverseInertiaTensor(const Math::mat3f& inverseInertiaTensor);

                void GetInverseInertiaTensor(Math::mat3f* inverseInertiaTensor) const;

                Math::mat3f GetInverseInertiaTensor() const;

                void GetInverseInertiaTensorWorld(Math::mat3f* inverseInertiaTensor) const;

                Math::mat3f GetInverseInertiaTensorWorld() const;

                void SetDamping(const float linearDamping, const float angularDamping);

                void SetLinearDamping(const float linearDamping);

                float GetLinearDamping() const;

                void SetAngularDamping(const float angularDamping);

                float GetAngularDamping() const;

                void SetPosition(const Math::vec3f& position);

                void SetPosition(const float x, const float y, const float z);

                void GetPosition(Math::vec3f* position) const;

                Math::vec3f GetPosition() const;

                /**
                 * Ustawienie orientacji ciała sztywnego.
                 * 
                 * @param orientation Nowa orientacja ciała sztywnego.
                 * 
                 * @note Dana orientacja nie musi być znormalizowana i może być zerem.
                 * Ta metoda automatycznie tworzy prawidłowy kwaternion rotacji z
                 * (0,0,0,0) mapowanym na (1,0,0,0).
                 */
                void SetOrientation(const Math::quaternionf& orientation);

                /**
                 * Ustawienie orientacji ciała sztywnego z podanych parametrów.
                 * 
                 * @param r Cześć rzeczywista kwaterniona orientacji.
                 * @param i Pierwsza część urojona kwateriona orientacji.
                 * @param j Druga część urojona kwaterniona orientacji.
                 * @param k Trzecia część urojona kwaterniona orientacji.
                 * 
                 * @note Dana orientacja nie musi być znormalizowana i może być zerem.
                 * Ta metoda automatycznie tworzy prawidłowy kwaternion rotacji z
                 * (0,0,0,0) mapowanym na (1,0,0,0).
                 */
                void SetOrientation(const float r, const float i, const float j, const float k);

                /**
                 * Wypełnienie podanego kwaterniona aktualnym kwaternionem orientacji
                 * ciała.
                 * 
                 * @param orientation Wskaźnik na kwaternion który ma zostać uzupełniony.
                 */
                void GetOrientation(Math::quaternionf* orientation) const;

                /**
                 * Pobranie orientacji ciała sztywnego.
                 * 
                 * @return Orientacja ciała sztywnego.
                 */
                Math::quaternionf GetOrientation() const;

                /**
                 * Wypełnienie podanej macierzy transformacją reprezentującą
                 * orientację ciała sztywnego.
                 * 
                 * @param matrix Wskaźnik na macierz do wypełnienia.
                 * 
                 * @note Transformacja wektora kierunku przez macierz zmienia
                 * przestrzeń z przestrzeni ciała na przestrzeń świata.
                 */
                void GetOrientation(Math::mat3f* matrix) const;

                /**
                 * Wypełnienie podanej tablicy transformacją reprezentującą
                 * orientację ciała sztywnego.
                 * 
                 * @param matrix Wskaźnik do tablicy do wypełnienia.
                 * 
                 * @note Transformacja wektora kierunku przez macierz zmienia
                 * przestrzeń z przestrzeni ciała na przestrzeń świata.
                 */
                void GetOrientation(float matrix[9]) const;

                /**
                 * Wypełnienie podanej macierzy transformacją reprezentującą
                 * położenie i orientację ciała sztywnego.
                 * 
                 * @param transform Wskaźnik na macierz do wypełnienia.
                 * 
                 * @note Transformacja wektora przez tą macierz zmienia przestrzeń
                 * z lokalnej na przestrzeń świata.
                 */
                void GetTransform(Math::mat4f* transform) const;

                /**
                 * Wypełnienie podanej tablicy transformacją reprezentującą
                 * położenie i orientację ciała sztywnego.
                 * 
                 * @param transform Wskaźnik na tablicę do wypełnienia.
                 * 
                 * @note Transformacja wektora przez tą macierz zmienia przestrzeń
                 * z lokalnej na przestrzeń świata.
                 */
                void GetTransform(float matrix[16]) const;

                /**
                 * Pobranie macierzy reprezentującej pozycję i orientację ciała sztywnego.
                 * 
                 * @return Macierz transformacji ciała sztywnego.
                 * 
                 * @note Transformacja wektora przez tą macierz zmienia przestrzeń
                 * z lokalnej na przestrzeń świata.
                 */
                Math::mat4f GetTransform() const;

                /**
                 * Konwersja podanego punktu z przestrzeni świata do lokalnej
                 * przestrzeni ciała sztywnego.
                 * 
                 * @param point Punkt do przekształcenia, w przestrzeni świata.
                 * 
                 * @return Skonwertowany punkt, w lokalnej przestrzeni.
                 */
                Math::vec3f GetPointInLocalSpace(const Math::vec3f& point) const;

                /**
                 * Konwersja podanego punktu z przestrzeni lokalnej ciała sztywnego
                 * do przestrzeni świata.
                 * 
                 * @param point Punkt do przekształcenia, w przestrzeni lokalnej.
                 * 
                 * @return Skonwertowany punkt, w przestrzeni świata.
                 */
                Math::vec3f GetPointInWorldSpace(const Math::vec3f& point) const;

                /**
                 * Konwersja podanego kierunku z przestrzeni świata do lokalnej
                 * przestrzeni ciała sztywnego.
                 * 
                 * @note Kiedy kierunek jest konwertowany pomiędzy klatkami,
                 * nie ma potrzeby translacji.
                 * 
                 * @param direction Kierunek do przekształcenia, w przestrzeni świata.
                 * 
                 * @return Skonwertowany kierunek, w przestrzeni lokalnej.
                 */
                Math::vec3f GetDirectionInLocalSpace(const Math::vec3f& direction) const;

                /**
                 * Konwersja podanego kierunku z przestrzeni lokalnej ciała sztywnego
                 * do przestrzeni świata.
                 * 
                 * @note Kiedy kierunek jest konwertowany pomiędzy klatkami, nie ma
                 * potrzeby translacji.
                 * 
                 * @param direction Kierunek do przekształcenia, w przestrzeni lokalnej.
                 * 
                 * @return Skonwertowany kierunek, w przestrzeni świata.
                 */
                Math::vec3f GetDirectionInWorldSpace(const Math::vec3f& direction) const;

                void SetVelocity(const Math::vec3f& velocity);
                void SetVelocity(const float x, const float y, const float z);

                void GetVelocity(Math::vec3f* velocity) const;
                Math::vec3f GetVelocity() const;

                void AddVelocity(const Math::vec3f& deltaVelocity);

                void SetRotation(const Math::vec3f& rotation);
                void SetRotation(const float x, const float y, const float z);

                void GetRotation(Math::vec3f* rotation) const;
                Math::vec3f GetRotation() const;

                void AddRotation(const Math::vec3f& deltaRotation);

                bool GetAwake() const;

                void SetAwake(const bool awake = true);

                bool GetCanSleep() const;

                void SetCanSleep(const bool canSleep = true);

                void GetLastFrameAcceleration(Math::vec3f* linearAcceleration) const;

                Math::vec3f GetLastFrameAcceleration() const;

                void ClearAccumulators();

                void AddForce(const Math::vec3f& force);

                void AddForceAtPoint(const Math::vec3f& force, const Math::vec3f& point);

                void AddForceAtBodyPoint(const Math::vec3f& force, const Math::vec3f& point);

                void AddTorque(const Math::vec3f& torque);

                void SetAcceleration(const Math::vec3f& acceleration);

                void SetAcceleration(const float x, const float y, const float z);

                void GetAcceleration(Math::vec3f* acceleration) const;

                Math::vec3f GetAcceleration() const;
            };
        }
    }
}

#endif	/* RIGIDBODY_HPP */

