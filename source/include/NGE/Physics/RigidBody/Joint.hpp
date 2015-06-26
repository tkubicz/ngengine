/* 
 * File:   Joint.hpp
 * Author: tku
 *
 * Created on 21 styczeń 2014, 16:23
 */

#ifndef JOINT_HPP
#define	JOINT_HPP

#include "NGE/Physics/RigidBody/ContactGenerator.hpp"
#include "NGE/Physics/RigidBody/RigidBody.hpp"
#include "NGE/Physics/RigidBody/Contact.hpp"

namespace NGE {
	namespace Physics {
		namespace RigidBody {

			/**
			 * Reprezentacja połączeń między ciałami sztywnymi. W silniku
			 * fizycznym może istnieć wiele różnych typów połączeń, które
			 * zmniejszają liczbę kątów w jakich mogą znajdować się dwa
			 * obiekty.
			 * To połączenie to podstawowe połączenie pozycji: każdy obiekt
			 * posiada swoją pozycję (podaną w przestrzeni obiektu), która
			 * zostanie utrzymana w tym samym miejscu w trakcie symulacji.
			 */
			class Joint : public ContactGenerator {
			  public:
				/**
				 * Dwa ciała sztywne połączone ze sobą.
				 */
				RigidBody* body[2];

				/**
				 * Względne położenie połączenia dla każdego obiektu,
				 * podane w przestrzeni lokalnej.
				 */
				Math::vec3f position[2];

				/**
				 * Maksymalne przemieszczenie połączenie, zanim połączenie
				 * nie zostanie uznane za przekroczone. Zwykle mała liczba.
				 */
				float error;

				/**
				 * Ustawienie połączenia.
				 * 
				 * @param a Pierwsze ciało sztywne.
				 * @param aPos Położenie pierwszego ciała sztywnego.
				 * @param b Drugie ciało sztywne.
				 * @param bPos Pozycja drugiego ciała sztywnego.
				 * @param error Maksymalne przemieszenie połączenia.
				 */
				void Set(RigidBody* a, const Math::vec3f& aPos,
						RigidBody* b, const Math::vec3f& bPos,
						float error);

				/**
				 * Wygenerowanie kontaktu potrzebne do przywrócenia połączenia,
				 * jeżeli zostało przekroczone.
				 * 
				 * @param contact Wskaźnik na pierwszy możliwy kontakt w tablicy.
				 * @param limit Maksymalna liczba kontaktów w tablicy która może
				 * zostać zapisana.
				 * 
				 * @return Liczba kontaktów które zostały zapisane w tablicy.
				 */
				unsigned AddContact(Contact* contact, unsigned limit) const;
			};
		}
	}
}

#endif	/* JOINT_HPP */

