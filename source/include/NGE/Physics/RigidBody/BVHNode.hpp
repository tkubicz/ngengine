/* 
 * File:   BVHNode.hpp
 * Author: tku
 *
 * Created on 27 styczeń 2014, 13:44
 */

#ifndef BVHNODE_HPP
#define	BVHNODE_HPP

#include "NGE/Physics/RigidBody/RigidBody.hpp"

namespace NGE {
	namespace Physics {
		namespace RigidBody {

			/**
			 * Klasa przechowująca potencjalne kontakty do sprawdzenia później.
			 */
			class PotentialContact {
			  public:
				/**
				 * Ciała które potencjalnie mogą być w kontakcie.
				 */
				RigidBody* body[2];
			};

			template <class BoundingVolumeClass> class BVHNode {
			  public:
				/**
				 * Węzły podrzędnę do tego węzła.
				 */
				BVHNode* children[2];

				/**
				 * 
				 */
				BoundingVolumeClass volume;

				/**
				 */
				RigidBody* body;

				/**
				 * Wskaźnik na węzęł wyżej drzewa.
				 */
				BVHNode* parent;

				BVHNode(BVHNode* parent, const BoundingVolumeClass& volume, RigidBody* body = NULL) : parent(parent), volume(volume), body(body) {
					children[0] = children[1] = NULL;
				}

				/**
				 * 
				 */
				~BVHNode();

				/**
				 * Sprawdzenie czy ten węzeł jest liściem - jest na samym dole hierarchii.
				 */
				bool IsLeaf() const {
					return (body != NULL);
				}

				/**
				 * Sprawdzenie potencjalnych kontaktów z tego węzła w dół drzewa, zapisując je
				 * do podanej tablicy (aż do osiągnięcia podanego limitu).
				 *
				 * @return Liczba potencjalnych kontaktów jaka została odnaleziona.
				 */
				unsigned GetPotentialContacts(PotentialContact* contacts, unsigned limit) const;

				/**
				 * Dodanie podanego ciała sztywnego, wraz z podaną bryłą ograniczającą do struktury.
				 * Wywołanie tej metody może skutkować wygenerowaniem dodatkowych węzłów drzewa.
				 */
				void Insert(RigidBody* body, const BoundingVolumeClass& volume);

			  protected:
				/**
				 * Sprawdzenie nakładania się pomiędzy węzłami drzewa.
				 *
				 * @note Bryły ograniczające powinny mieć zaimplementowane metody
				 * sprawdzania nakładania się dla siebie.
				 */
				bool Overlaps(const BVHNode<BoundingVolumeClass>* other) const;

				/**
				 * Sprawdzenie potencjalnych kontaktów pomiędzy tym węzłem i podanym węzłem.
				 * Potencjalne kontakty zapisywane są do podanej tablicy (aż do osiągnięcia
				 * podanego limitu).
				 *
				 * @return Liczba potencjalnych kontaktów jaka została odnaleziona.
				 */
				unsigned GetPotentialContactsWith(const BVHNode<BoundingVolumeClass>* other, PotentialContact* contacts, unsigned limit) const;

				/**
				 * 
				 */
				void RecalculateBoundingVolume(bool recurse = true);
			};

			template <class BoundingVolumeClass> bool BVHNode<BoundingVolumeClass>::Overlaps(const BVHNode<BoundingVolumeClass>* other) const {
				volume.Overlaps(other->volume);
			}

			template <class BoundingVolumeClass> void BVHNode<BoundingVolumeClass>::Insert(RigidBody* newBody, const BoundingVolumeClass& newVolume) {
				// Jeśli jesteśmy liściem, to jedyną możliwością jest utworzenie dwóch potomków i przeniesienie obiektu do jednego z nich.
				if (IsLeaf()) {
					// Pierwszy potomek jest kopią aktualnego węzła.
					children[0] = new BVHNode<BoundingVolumeClass>(this, volume, body);

					// Drugi potomek przechowuje nowy obiekt.
					children[1] = new BVHNode<BoundingVolumeClass>(this, newVolume, newBody);

					// Usunięcie obiektu z aktualnego węzła
					this->body = NULL;

					// Przeliczenie bryły ograniczającej
					RecalculateBoundingVolume();
				}					// W innym wypadku musimy rozważyć który do którego potmoka mamy
					// dodać ciało. Dodamy je do tego, które zwiększy się w najmniejszym
					// stopniu po dodaniu.
				else {
					if (children[0]->volume.GetGrowth(newVolume) < children[1]->volume.GetGrowth(newVolume))
						children[0]->Insert(newBody, newVolume);
					else
						children[1]->Insert(newBody, newVolume);
				}
			}

			template <class BoundingVolumeClass> BVHNode<BoundingVolumeClass>::~BVHNode() {
				// Jeżeli węzeł nie posiada rodzica, to ignorujemy węzły z tego samego poziomu.
				if (parent) {
					// Odnalezienie węzła z tego samego poziomu.
					BVHNode<BoundingVolumeClass>* sibling;
					if (parent->children[0] == this)
						sibling = parent->children[1];
					else
						sibling = parent->children[0];

					// Zapisanie danych do rodzica
					parent->volume = sibling->volume;
					parent->body = sibling->body;
					parent->children[0] = sibling->children[0];
					parent->children[1] = sibling->children[1];

					// Usunięcie węzła z tego samego poziomu
					sibling->parent = NULL;
					sibling->body = NULL;
					sibling->children[0] = NULL;
					sibling->children[1] = NULL;
					delete sibling;

					// Przeliczenie bryły ograniczającej rodzica
					parent->RecalculateBoundingVolume();
				}

				// Usunięcie dzieci
				if (children[0]) {
					children[0]->parent = NULL;
					delete children[0];
				}

				if (children[1]) {
					children[1]->parent = NULL;
					delete children[1];
				}
			}

			template <class BoundingVolumeClass> void BVHNode<BoundingVolumeClass>::RecalculateBoundingVolume(bool recurse) {
				if (IsLeaf())
					return;

				volume = BoundingVolumeClass(children[0]->volume, children[1]->volume);

				if (parent)
					parent->RecalculateBoundingVolume(true);
			}

			template <class BoundingVolumeClass> unsigned BVHNode<BoundingVolumeClass>::GetPotentialContacts(PotentialContact* contacts, unsigned limit) const {
				if (IsLeaf() || limit == 0)
					return 0;

				return children[0]->GetPotentialContactsWith(children[1], contacts, limit);
			}

			template <class BoundingVolumeClass> unsigned BVHNode<BoundingVolumeClass>::GetPotentialContactsWith(const BVHNode<BoundingVolumeClass>* other, PotentialContact* contacts, unsigned limit) const {
				// Jeżeli obiekty się nie nakładają, lub nie mamy miejsca na kontakt
				if (!Overlaps(other) || limit == 0)
					return 0;

				// Jeżeli oba obiekty to liście, to mamy potencjalny kontakt
				if (IsLeaf() && other->IsLeaf()) {
					contacts->body[0] = body;
					contacts->body[1] = other->body;
					return 1;
				}

				if (other->IsLeaf() || (!IsLeaf() && (volume.GetSize() >= other->volume.GetSize()))) {
					unsigned count = children[0]->GetPotentialContactsWith(other, contacts, limit);

					if (limit > count)
						return count + children[1]->GetPotentialContactsWith(other, contacts + count, limit - count);
					else
						return count;
				} else {
					unsigned count = GetPotentialContactsWith(other->children[0], contacts, limit);

					if (limit > count)
						return count + GetPotentialContactsWith(other->children[1], contacts + count, limit - count);
					else
						return count;
				}
			}
		}
	}
}



#endif	/* BVHNODE_HPP */

