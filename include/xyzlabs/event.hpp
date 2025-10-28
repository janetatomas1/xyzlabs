
#pragma once

#include <string>
#include <memory>
#include <functional>

struct Event {
    std::string label;
    Event(const std::string &label): label(label) {};
};

using event_ptr = std::unique_ptr<Event>;
using callback = std::move_only_function<void(std::unique_ptr<Event> event)>;
using action = std::move_only_function<void()>;
