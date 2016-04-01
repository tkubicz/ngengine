#include "NGE/Events/AbstractEventBus.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"
using namespace NGE::Events;

static AbstractEventBus* eventManager = NULL;

AbstractEventBus::AbstractEventBus(const std::string& name, bool setAsGlobal) {
	if (setAsGlobal) {
		if (eventManager) {
			log_warn("Attempting to create two global managers! The old one will be destroyed and overwritten with this one");
			delete eventManager;
		}

		eventManager = this;
	}
}

AbstractEventBus::~AbstractEventBus() {
	if (eventManager == this) {
		log_debug("Destroying global eventManager");
		eventManager = nullptr;
	}
}

AbstractEventBus* AbstractEventBus::Get() {
	if (eventManager == nullptr) {
		log_debug("Attempting to get not initialised global event manager");
		return nullptr;
	}
	return eventManager;
}



