/* 
 * File:   LoadTextureEvent.hpp
 * Author: tku
 *
 * Created on 5 April 2016, 10:47
 */

#ifndef LOADTEXTUREEVENT_HPP
#define LOADTEXTUREEVENT_HPP

#include <pugixml.hpp>
#include "NGE/Events/BaseEventData.hpp"

using namespace NGE::Events;

namespace NGE {
	namespace Media {
		namespace Images {

			class LoadTextureEvent : public BaseEventData {
			  private:
				pugi::xml_node xmlNode;
				bool successful;

			  public:
				static const EventType eventType;

				LoadTextureEvent(const pugi::xml_node& xmlNode, const float timeStamp = 0.0f) : BaseEventData(timeStamp), xmlNode(xmlNode) { }

				const EventType& GetEventType() const override {
					return eventType;
				}

				IEventDataPtr Copy() const override {
					return IEventDataPtr(new LoadTextureEvent(xmlNode));
				}

				const std::string GetName() const override {
					return "LoadTextureEvent";
				}

				pugi::xml_node& GetXmlNode() {
					return xmlNode;
				}

				bool IsSuccessful() const {
					return successful;
				}

				void SetSuccessful(bool successful) {
					this->successful = successful;
				}

			};
		}
	}
}

#endif /* LOADTEXTUREEVENT_HPP */

