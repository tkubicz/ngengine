/* 
 * File:   ActorComponenet.hpp
 * Author: tku
 *
 * Created on 9 May 2016, 16:17
 */

#ifndef ACTORCOMPONENET_HPP
#define ACTORCOMPONENET_HPP

#include <functional>
#include "Actor.hpp"

namespace NGE {
	namespace Actors {

		class ActorComponent {
			friend class ActorFactory;

		  protected:
			StrongActorPtr owner;

		  public:

			virtual ~ActorComponent();

			virtual bool Init() = 0;

			virtual void PostInit() { }

			virtual void Update(int deltaMs) { }

			virtual void OnChanged() { }

			virtual ComponentId GetId() const;

			virtual const std::string GetName() const = 0;

			virtual ComponentId GetIdFromName(const std::string& componenetName) const;

		  private:

			void SetOwner(StrongActorPtr owner);
		};
	}
}

#endif /* ACTORCOMPONENET_HPP */

