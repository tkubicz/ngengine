/* 
 * File:   CollisionData.hpp
 * Author: tku
 *
 * Created on 15 styczeń 2014, 13:39
 */

#ifndef COLLISIONDATA_HPP
#define	COLLISIONDATA_HPP

#include "NGE/Physics/RigidBody/Contact.hpp"

namespace NGE {
	namespace Physics {
		namespace RigidBody {

			class CollisionData {
			  public:
				Contact* contactArray;

				/**
				 * Tablica kontaktów służąca do ich zapisywania.
				 */
				Contact* contacts;

				/**
				 * Maksymalna liczba kontaktów jakie tablica może przyjąć.
				 */
				int contactsLeft;

				/**
				 * Liczba kontaktów znaleziona do tej pory.
				 */
				unsigned contactCount;

				/**
				 * Wartość tarcia do zapisania w każdej kolizji.
				 */
				float friction;

				/**
				 * Wartość restytucji do zapisania w każdej kolizji.
				 */
				float restitution;

				/**
				 * Wartość tolerancji kolizji - nawet obiekty nie zderzające
				 * się ze sobą, a będące w granicach powinny mieć wygenerowaną
				 * kolizję.
				 */
				float tolerance;

				/**
				 * Sprawdzenie czy istnieją jeszcze dostępne kontakty.
				 * TODO: Przenieść implementację do pliku *.cpp
				 */
				bool HasMoreContacts() {
					return contactsLeft > 0;
				}

				/**
				 * Zresetowanie informacji.
				 */
				void Reset(unsigned maxContacts) {
					contactsLeft = maxContacts;
					contactCount = 0;
					contacts = contactArray;
				}

				void AddContacts(unsigned count) {
					// Zmniejszenie liczby pozostałych kontaktów 
					// i zwiększenie liczby użytych
					contactsLeft -= count;
					contactCount += count;

					// Przesunięcie tablicy w przód
					contacts += count;
				}
			};
		}
	}
}

#endif	/* COLLISIONDATA_HPP */

