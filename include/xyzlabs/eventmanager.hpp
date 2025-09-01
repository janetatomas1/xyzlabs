
#ifndef ACTIONMANAGER_HPP
#define ACTIONMANAGER_HPP

#include <boost/unordered/unordered_flat_map.hpp>
#include <moodycamel/concurrentqueue.h>
#include <vector>
#include <memory>
#include <functional>


struct Event {
    std::string label;
    Event(const std::string &label): label(label) {};
};

struct Callback {
    virtual void operator()(std::unique_ptr<Event> ptr) {};
};

using callback_ptr = std::unique_ptr<std::function<void(std::unique_ptr<Event>)>>;

class EventManager {
    moodycamel::ConcurrentQueue<std::unique_ptr<Event>> events_;
    boost::unordered::unordered_flat_map <std::string, callback_ptr> callbacks_;

public:
    EventManager() = default;
    void dispatch();
    void add_event(std::unique_ptr<Event> ptr);
    void subscribe(const std::string &label, callback_ptr callback);
    void unsubscribe(const std::string &label);
};

#endif
