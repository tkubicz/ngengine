#include "NGE/Events/EventManager.hpp"
#include "NGE/Events/EventBus.hpp"
using namespace NGE::Events;

EventManager::EventManager() {
    CreateGlobalEventBus();
}

EventManager::~EventManager() {
    for (auto& bus : eventBusMap) {
        bus.second.reset();
    }
    eventBusMap.clear();
    globalEventBus.reset();
}

void EventManager::CreateGlobalEventBus() {
    std::shared_ptr<AbstractEventBus> globalEventBusInstance = std::make_shared<AbstractEventBus>("GLOBAL");
    globalEventBus = globalEventBusInstance;
}

std::weak_ptr<AbstractEventBus> EventManager::Create(const std::string& name) {
    std::shared_ptr<AbstractEventBus> eventBusPtr = std::make_shared<AbstractEventBus>(name);
    eventBusMap.insert(std::pair<std::string, std::shared_ptr < AbstractEventBus >> (name, std::shared_ptr<AbstractEventBus>(new EventBus(name))));
    return eventBusPtr;
}

bool EventManager::Delete(const std::string& name) { }
