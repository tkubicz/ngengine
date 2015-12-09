/* 
 * File:   ActorInterfaces.hpp
 * Author: tku
 *
 * Created on 28 listopad 2015, 01:05
 */

#ifndef ACTORINTERFACES_HPP
#define ACTORINTERFACES_HPP

namespace NGE {
    namespace Actors {
        class Actor;
        class ActorComponent;

        typedef unsigned int ActorId;
        typedef unsigned int ComponentId;

        typedef std::shared_ptr<Actor> StrongActorPtr;
        typedef std::weak_ptr<Actor> WeakActorPtr;
        typedef std::shared_ptr<ActorComponent> StrongActorComponentPtr;
        typedef std::weak_ptr<ActorComponent> WeakActorComponentPtr;
    }
}

#endif /* ACTORINTERFACES_HPP */

