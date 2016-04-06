/* 
 * File:   GetShaderEvent.hpp
 * Author: tku
 *
 * Created on 5 April 2016, 21:36
 */

#ifndef GETSHADEREVENT_HPP
#define GETSHADEREVENT_HPP

#include "NGE/Events/BaseEventData.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"

using namespace NGE::Events;

namespace NGE {
	namespace Media {
		namespace Shaders {

			class GetShaderEvent : public BaseEventData {
				friend class ShaderManager;

			  private:
				std::string shaderName;
				bool successful;
				GLSLProgram* shader;

			  public:
				static const EventType eventType;

				GetShaderEvent(const std::string& shaderName, const float timeStamp = 0.0f) : BaseEventData(timeStamp), shaderName(shaderName), shader(nullptr) { }

				const EventType& GetEventType() const override {
					return eventType;
				}

				const std::string GetName() const override {
					return "GetShaderEvent";
				}

				IEventDataPtr Copy() const override {
					return NGE::Events::IEventDataPtr(new GetShaderEvent(shaderName));
				}

				GLSLProgram* GetShader() const {
					return shader;
				}

				std::string GetShaderName() const {
					return shaderName;
				}

				bool IsSuccessful() const {
					return successful;
				}
			};
		}
	}
}

#endif /* GETSHADEREVENT_HPP */

