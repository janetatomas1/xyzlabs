
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
            if(listeners_.contains(event->label)) {
                for(auto &ref: listeners_.at(event->label)) {
                    event->run(ref);
                }
            }
        }
    }
}

void EventManager::subscribe(const std::string &label, Listener *listener) {
    if(listeners_.contains(label)) {
        listeners_[label].push_back(listener);
    } else {
        listeners_[label] = {listener};
    }
}

void EventManager::unsubscribe(const std::string &label, Listener *listener) {
    if(listeners_.contains(label)) {
        auto &ptrs = listeners_[label];
        std::remove(ptrs.begin(), ptrs.end(), listener);
    }
}
