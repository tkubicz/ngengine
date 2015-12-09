/* 
 * File:   Actor.hpp
 * Author: tku
 *
 * Created on 29 listopad 2015, 04:26
 */

#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "NGE/Actors/ActorInterfaces.hpp"

namespace NGE {
    namespace Actors {

        class Actor {
          public:
            typedef std::map<ComponentId, StrongActorComponentPtr> ActorComponents;
            typedef std::string ActorType;

          private:
            /**
             * Unique ID for the Actor.
             */
            ActorId id;

            /**
             * All components this actor has.
             */
            ActorComponents components;

            /**
             * Type of the actor.
             */
            ActorType type;

          public:
            explicit Actor(ActorId id);
            virtual ~Actor();

            bool Initialise(pugi::xml_node* data);
            void PostInit();
            void Terminate();
            void Update(int deltaTimeMs);

            std::string ToXml();

            ActorId GetId() const;
            ActorType GetType() const;
            const ActorComponents* GetComponents();
            template <class ComponentType> std::weak_ptr<ComponentType> GetComponent(ComponentId id);
            template <class ComponentType> std::weak_ptr<ComponentType> GetComponent(const std::string& name);

            void AddComponent(StrongActorComponentPtr componenet);
        };
    }
}

#endif /* ACTOR_HPP */

