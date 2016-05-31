/* 
 * File:   Actor.hpp
 * Author: tku
 *
 * Created on 29 November 2015, 04:26
 */

#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <map>
#include <pugixml.hpp>
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

			/**
			 * The XML file from which this actor was initialised.
			 */
			std::string resource;

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

			template <class ComponentType> std::weak_ptr<ComponentType> GetComponent(ComponentId id) {
				auto find = components.find(id);
				if (find != components.end()) {
					StrongActorComponentPtr base(find->second);
					// Cast to subclass version of the pointer.
					std::shared_ptr<ComponentType> sub(std::static_pointer_cast<ComponentType>(base));
					// Convert strong pointer to weak pointer.
					std::weak_ptr<ComponentType> weakSub(sub);
					// Return the weak pointer.
					return weakSub;
				} else {
					return std::weak_ptr<ComponentType>();
				}
			}

			template <class ComponentType> std::weak_ptr<ComponentType> GetComponent(const std::string& name) { }

			void AddComponent(StrongActorComponentPtr componenet);
		};
	}
}

#endif /* ACTOR_HPP */

