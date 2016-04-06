/* 
 * File:   LoadShaderEvent.hpp
 * Author: tku
 *
 * Created on 5 April 2016, 21:36
 */

#ifndef LOADSHADEREVENT_HPP
#define LOADSHADEREVENT_HPP

#include <pugixml.hpp>
#include "NGE/Events/BaseEventData.hpp"

using namespace NGE::Events;

namespace NGE {
	namespace Media {
		namespace Shaders {

			class LoadShaderEvent : public BaseEventData {
				friend class ShaderManager;

			  private:
				std::string programName;
				std::string fileName;
				bool successful;

			  public:
				static const EventType eventType;

				LoadShaderEvent(const std::string& programName, const std::string& fileName, const float timeStamp = 0.0f)
				: BaseEventData(timeStamp), programName(programName), fileName(fileName) { }

				const EventType& GetEventType() const override {
					return eventType;
				}

				IEventDataPtr Copy() const override {
					return IEventDataPtr(new LoadShaderEvent(programName, fileName));
				}

				const std::string GetName() const override {
					return "LoadShaderEvent";
				}

				bool IsSuccessful() const {
					return successful;
				}

				std::string GetFileName() const {
					return fileName;
				}

				std::string GetProgramName() const {
					return programName;
				}

			};
		}
	}
}

#endif /* LOADSHADEREVENT_HPP */

