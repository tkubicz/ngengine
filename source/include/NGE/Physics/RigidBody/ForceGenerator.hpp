/* 
 * File:   ForceGenerator.hpp
 * Author: tku
 *
 * Created on 8 styczeń 2014, 23:42
 */

#ifndef FORCEGENERATOR_HPP
#define	FORCEGENERATOR_HPP

#include <vector>

#include "NGE/Physics/RigidBody/RigidBody.hpp"
#include "NGE/Math/Vector3.hpp"
#include "NGE/Math/Matrix3.hpp"
#include "NGE/Math/Quaternion.hpp"

namespace NGE
{
    namespace Physics
    {
        namespace RigidBody
        {
            /**
             * Generator siły może dodawać siły do jednego lub więcej obiektów.
             */
            class ForceGenerator
            {
              public:
                /**
                 * Tą metodą należy zaimplementować tak by liczyła i aktualizowała
                 * siłę jaka działa na podane ciało.
                 * @param body Ciało sztywne na którym zostanie zastosowana siła.
                 * @param duration Czas.
                 */
                virtual void UpdateForce(RigidBody* body, float duration) = 0;
            };
            
            /**
             * Generator siły reprezentujący siłę grawitacji. Jedna instancja może
             * być używana dla wielu obiektów.
             */
            class Gravity : public ForceGenerator
            {
              protected:
                /**
                 * Przyśpieszenie grawitacyjne.
                 */
                Math::vec3f gravity;
                
              public:
                /**
                 * Utworzenie generatora siły z podanym przyśpieszeniem.
                 * 
                 * @param gravity Przyśpieszenie grawitacyjne.
                 */
                Gravity(const Math::vec3f& gravity);
                
                /**
                 * Zastosowanie grawitacji na podane ciało.
                 * 
                 * @param body Ciało sztywne na jakie ma działać grawitacja.
                 * @param duration Czas.
                 */
                virtual void UpdateForce(RigidBody* body, float duration);
            };
            
            /**
             * Generator siły reprezentujący sprężystość.
             * TODO: Uzupełnić komentarze
             */
            class Spring : public ForceGenerator
            {
              protected:
                Math::vec3f connectionPoint;                
                Math::vec3f otherConnectionPoint;
                RigidBody* other;
                float springConstant;
                float restLength;
                
              public:
                Spring(const Math::vec3f& locationConnectionPoint, RigidBody* other, const Math::vec3f& otherConnectionPoint, float springConstant, float restLength);
                
                virtual void UpdateForce(RigidBody* body, float duration);
            };
            
            /**
             * Generator siły reprezentujący siłę aerodynamiczną.
             */
            class Aero : public ForceGenerator
            {
              protected:
                /**
                 * Tensor aerodynamiczny dla powierzchni w przestrzeni obiektu.
                 */
                Math::mat3f tensor;
                
                /**
                 * Pozycja względem powierzchni aerodynamicznej w przestrzeni obiektu.
                 */
                Math::vec3f position;
                
                /**
                 * Wskaźnik na wektor zawierający prędkość wiatru. Taka implementacja
                 * jest łatwiejsza niż utrzymywanie prędkości wiatru osobno w każdym
                 * obiekcie i aktualizacjia w razie modyfikacji.
                 */
                const Math::vec3f* windspeed;
                
                /**
                 * Użycie podanego tensora do zastosowania siły na podane ciało.
                 * Metoda robi to samo co UpdateForce, tylko wykorzystuje podany
                 * tensor.
                 * 
                 * @param body Ciało na które ma działać siła.
                 * @param duration Czas.
                 * @param tensor Tensor który ma zostać zastosowany.
                 */
                void UpdateForceFromTensor(RigidBody* body, float duration, const Math::mat3f& tensor);
                
              public:
                /**
                 * Utworzenie nowego generatora z podanych parametrów.
                 * 
                 * @param tensor Tensor.
                 * @param position Pozycja.
                 * @param windspeed Szybkość wiatru.
                 */
                Aero(const Math::mat3f& tensor, const Math::vec3f& position, const Math::vec3f* windspeed);
                
                /**
                 * Zastosowanie siły na podane ciało.
                 * 
                 * @param body Ciało na które ma działać siła.
                 * @param duration Czas.
                 */
                virtual void UpdateForce(RigidBody* body, float duration);
            };
            
            /**
             * Generator kontrolowanej siły aerodynamicznej. Wymaga
             * trzech tensorów bezwładności, dwóch dla wartości 
             * granicznych i jednego dla stanu spoczynku. Tensor stanu
             * spoczynku jest dziedziczony z klasy Aero.
             */
            class AeroControl : public Aero
            {
              protected:
                Math::mat3f maxTensor;
                Math::mat3f minTensor;
                
                /**
                 * Aktualna pozycja kontrolera dla tej powierzchni. Ta
                 * wartość powinna znajdować się w przedziałe <-1,1>.
                 * -1 - użycie minTensor
                 * 0 - użycie tensor (stan spoczynku)
                 * 1 - użycie maxTensor
                 */
                float controlSetting;
                
              private:
                /**
                 * Wyliczenie tensora dla aktualnej wartości kontrolera.
                 * 
                 * @return Aktualny tensor.
                 */
                Math::mat3f GetTensor();
                
              public:
                /**
                 * Utworzenie nowej powierzchni aerodynamicznej z kontrolerem w zależności
                 * od podanych parametrów.
                 * 
                 * @param base
                 * @param min
                 * @param max
                 * @param position
                 * @param windspeed
                 */
                AeroControl(const Math::mat3f& base, const Math::mat3f& min, const Math::mat3f& max, const Math::vec3f& position, const Math::vec3f* windspeed);
                
                /**
                 * Ustawienie pozycji kontrolera. Ta
                 * wartość powinna znajdować się w przedziałe <-1,1>.
                 * -1 - użycie minTensor
                 * 0 - użycie tensor (stan spoczynku)
                 * 1 - użycie maxTensor
                 * Wartości z poza przedziału nie są obsługiwane.
                 * 
                 * @param value Wartość z przedziału <-1,1>
                 */
                void SetControl(float value);
                
                /**
                 * Zastosowanie siły na podane ciało.
                 * 
                 * @param body Ciało na które ma działać siła.
                 * @param duration Czas.
                 */
                virtual void UpdateForce(RigidBody* body, float duration);
            };
            
            class ForceRegistry
            {
              protected:
                
                struct ForceRegistration
                {
                    RigidBody* body;
                    ForceGenerator* fg;
                };
                
                typedef std::vector<ForceRegistration> Registry;
                Registry registrations;
                
              public:
                void Add(RigidBody* body, ForceGenerator* fg);
                void Remove(RigidBody* body, ForceGenerator* fg);
                
                void Clear();
                
                void UpdateForces(float duration);
            };
        }
    }
}

#endif	/* FORCEGENERATOR_HPP */

