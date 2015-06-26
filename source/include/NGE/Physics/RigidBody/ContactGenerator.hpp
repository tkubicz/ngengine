/* 
 * File:   ContactGenerator.hpp
 * Author: tku
 *
 * Created on 21 styczeń 2014, 16:25
 */

#ifndef CONTACTGENERATOR_HPP
#define	CONTACTGENERATOR_HPP

namespace NGE {
	namespace Physics {
		namespace RigidBody {
			class Contact;

			/**
			 * Interfejs dla generatorów kontaktu dla ciał sztywnych
			 */
			class ContactGenerator {
			  public:
				/**
				 * Wypełnienie podanej struktury kontaktu przez utworzony kontakt.
				 * @param contact Wskaźnik na pierwszy możliwy kontakt w tablicy.
				 * @param limit Maksymalna liczba kontaktów w tablicy która może
				 * zostać zapisana.
				 * @return Liczba kontaktów które zostały zapisane w tablicy.
				 */
				virtual unsigned AddContact(Contact* contact, unsigned limit) const = 0;
			};
		}
	}
}

#endif	/* CONTACTGENERATOR_HPP */

