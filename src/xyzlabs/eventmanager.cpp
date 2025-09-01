
#include <algorithm>

#include "xyzlabs/eventmanager.hpp"

void EventManager::add_event(std::unique_ptr<Event> event) {
    events_.enqueue(std::move(event));
}

void EventManager::dispatch() {
    std::unique_ptr<Event> event = nullptr;
    bool eventFound = false;

    while(true) {
        eventFound = events_.try_dequeue(event);

        if(!eventFound) {
            return;
        } else {
            if(callbacks_.contains(event->label)) {
                auto &ptr = callbacks_[event->label];
                (*ptr)(std::move(event));
            }
        }
    }
}

void EventManager::subscribe(const std::string &label, callback_ptr callback) {
    callbacks_[label] = std::move(callback);
}

void EventManager::unsubscribe(const std::string &label) {
    if(callbacks_.contains(label)) {
        callbacks_.erase(label);
    }
}
