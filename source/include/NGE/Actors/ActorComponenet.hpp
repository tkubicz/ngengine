/* 
 * File:   ActorComponenet.hpp
 * Author: tku
 *
 * Created on 9 May 2016, 16:17
 */

#ifndef ACTORCOMPONENET_HPP
#define ACTORCOMPONENET_HPP

#include "Actor.hpp"

namespace NGE {
	namespace Actors {

		class ActorComponent {
			friend class ActorFactory;

		  protected:
			StrongActorPtr owner;

		  public:

			virtual ~ActorComponent() {
				owner.reset();
			}

			virtual bool Init() = 0;

			virtual void PostInit() { }

			virtual void Update(int deltaMs) { }

			virtual void OnChanged() { }

			virtual ComponentId GetId() const {
				//return GetIdFromName(GetName());
			}

			virtual const char* GetName() const = 0;

			virtual ComponentId GetIdFromName(const char* componenetName) {
				//void* rawId = Hash
			}
		};
	}
}

#endif /* ACTORCOMPONENET_HPP */

