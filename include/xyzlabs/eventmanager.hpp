
#ifndef ACTIONMANAGER_HPP
#define ACTIONMANAGER_HPP

#include <boost/unordered/unordered_flat_map.hpp>
#include <moodycamel/concurrentqueue.h>
#include <vector>
#include <memory>

struct Listener{};
struct Event {
    std::string label;
    virtual void run(Listener *listener) {}; 
};

class EventManager {
    moodycamel::ConcurrentQueue<std::unique_ptr<Event>> events_;
    boost::unordered::unordered_flat_map <std::string, std::vector<Listener *>> listeners_;

public:
    EventManager() = default;
    void dispatch();
    void add_event(std::unique_ptr<Event> ptr);
    void subscribe(const std::string &label, Listener *listener);
    void unsubscribe(const std::string &label, Listener *listener);
};

#endif
