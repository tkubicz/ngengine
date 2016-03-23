#include "NGE/Events/IEventManager.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"
using namespace NGE::Events;

static IEventManager* eventManager = NULL;

IEventManager::IEventManager(const std::string& name, bool setAsGlobal) {
    if (setAsGlobal) {
        if (eventManager) {
            log_error("Attempting to create two global managers! The old one will be destroyed and overwritten with this one");
            delete eventManager;
        }

        eventManager = this;
    }
}

IEventManager::~IEventManager() {
    if (eventManager == this) {
        log_info("Destroying global eventManager");
        eventManager = nullptr;
    }
}

IEventManager* IEventManager::Get() {
    if (eventManager == nullptr) {
        log_info("Attempting to get uninitalised global event manager");
        return nullptr;
    }
    return eventManager;
}



