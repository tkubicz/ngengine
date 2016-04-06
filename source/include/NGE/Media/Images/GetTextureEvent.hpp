/* 
 * File:   GetTextureEvent.hpp
 * Author: tku
 *
 * Created on 5 April 2016, 11:56
 */

#ifndef GETTEXTUREEVENT_HPP
#define GETTEXTUREEVENT_HPP

#include "NGE/Events/BaseEventData.hpp"
#include "NGE/Media/Images/Texture.hpp"

using namespace NGE::Events;

namespace NGE {
	namespace Media {
		namespace Images {

			class GetTextureEvent : public BaseEventData {
			  private:
				std::string textureName;
				bool successful;
				Texture* texture;

			  public:
				static const EventType eventType;

				GetTextureEvent(const std::string& textureName, const float timeStamp = 0.0f) : BaseEventData(timeStamp), textureName(textureName), texture(nullptr) { }

				const EventType& GetEventType() const override {
					return eventType;
				}

				const std::string GetName() const {
					return "GetTextureEvent";
				}

				IEventDataPtr Copy() const override {
					return IEventDataPtr(new GetTextureEvent(textureName));
				}

				bool IsSuccessful() const {
					return successful;
				}

				void SetSuccessful(bool successful) {
					this->successful = successful;
				}

				Texture* GetTexture() const {
					return texture;
				}

				void SetTexture(Texture* texture) {
					this->texture = texture;
				}

				std::string GetTextureName() const {
					return textureName;
				}

			};
		}
	}
}

#endif /* GETTEXTUREEVENT_HPP */

