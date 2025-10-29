
#include <algorithm>
#include <spdlog/spdlog.h>

#include "xyzlabs/event.hpp"
#include "xyzlabs/eventmanager.hpp"

namespace xyzlabs {

void EventManager::add_event(event_ptr event) {
    spdlog::info("Adding event {}", event->label);
    events_.enqueue(std::move(event));
}

void EventManager::add_action(action act) {
    actions_.enqueue(std::move(act));
}

void EventManager::dispatch() {
    event_ptr event = nullptr;
    bool eventFound = false;

    while(true) {
        eventFound = events_.try_dequeue(event);

        if(eventFound) {
            if(callbacks_.contains(event->label)) {
                auto &ptr = callbacks_[event->label];
                ptr(std::move(event));
            }
        } else {
            break;
        }
    }

    action act = nullptr;
    bool actionFound = false;
    while(true) {
        actionFound = actions_.try_dequeue(act);
        if(actionFound) {
            act();
        } else {
            break;
        }
    }
}

void EventManager::subscribe(const std::string &label, callback call) {
    callbacks_[label] = std::move(call);
    spdlog::info("Subscribed to event {}", label);
}

void EventManager::unsubscribe(const std::string &label) {
    if(callbacks_.contains(label)) {
        callbacks_.erase(label);
        spdlog::info("Unsubscribed from event {}", label);
    }
}

}