/* 
 * File:   LuaScriptEvent.hpp
 * Author: tku
 *
 * Created on 14 March 2016, 01:33
 */

#ifndef LUASCRIPTEVENT_HPP
#define LUASCRIPTEVENT_HPP

#include <map>
#include <kaguya/kaguya.hpp>
#include "NGE/Tools/Logger/NewLogger.hpp"
#include "NGE/Events/IEventData.hpp"
#include "NGE/Events/BaseEventData.hpp"

namespace NGE {
	namespace Scripting {

		class LuaScriptEvent : public NGE::Events::BaseEventData {
		  private:
			static constexpr const NGE::Events::EventType eventType = 0x0f4bf9b3;

		  protected:
			std::map<std::string, std::string> scriptData;
			kaguya::LuaTable table;

		  public:

			explicit LuaScriptEvent() { }

			virtual const NGE::Events::EventType& GetEventType() const override {
				return eventType;
			}

			virtual NGE::Events::IEventDataPtr Copy() const override {
				return NGE::Events::IEventDataPtr(new LuaScriptEvent());
			}

			virtual const std::string GetName() const override {
				return "LuaScriptEvent";
			}

			static void RegisterScriptClass();

			static void RegisterEventTypeWithScript(const std::string& key, NGE::Events::EventType type);

			void BuildEventData(kaguya::LuaTable self) {
				if (self.isNilref()) {
					log_warn("self is nilref");
					return;
				}
				scriptData.clear();
				scriptData = self.map<std::string, std::string>();
				table = self;

				for (auto& kv : scriptData) {
					log_debug("key = '{}', value = '{}'", kv.first, kv.second);
				}
			}
		};
	}
}

#endif /* LUASCRIPTEVENT_HPP */

