#include "NGE/Events/IEventManager.hpp"
#include "NGE/Tools/Logger.hpp"
using namespace NGE::Events;

static IEventManager* eventManager = NULL;

IEventManager::IEventManager(const std::string& name, bool setAsGlobal) {
    if (setAsGlobal) {
        if (eventManager) {
            nge_log_error("IEventManager --> Attempting to create two global managers! The old one will be destroyed and overwritten with this one");
            delete eventManager;
        }

        eventManager = this;
    }
}

IEventManager::~IEventManager() {
    if (eventManager == this) {
        nge_log_info("IEventManager --> Destroying global eventManager");
        eventManager = nullptr;
    }
}

IEventManager* IEventManager::Get() {
    if (eventManager == nullptr) {
        nge_log_error("IEventManager --> Attempting to get uninitalized global event manager");
        return nullptr;
    }
    return eventManager;
}



