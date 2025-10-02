
#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <boost/unordered/unordered_flat_map.hpp>
#include <moodycamel/concurrentqueue.h>
#include <vector>
#include <memory>

#include "xyzlabs/event.hpp"

class EventManager {
    moodycamel::ConcurrentQueue<event_ptr> events_;
    moodycamel::ConcurrentQueue<action> actions_;
    boost::unordered::unordered_flat_map <std::string, callback> callbacks_;

public:
    EventManager() = default;
    void dispatch();
    void add_event(event_ptr event);
    void add_action(action act);
    void subscribe(const std::string &label, callback call);
    void unsubscribe(const std::string &label);
};

#endif
