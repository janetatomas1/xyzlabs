#pragma once

#include <string>
#include <memory>
#include <functional>

namespace xyzlabs {

/**
 * @brief Represents a generic event in the system.
 *
 * Each event has a textual label that describes its type or purpose.
 */
struct Event {
    std::string label;
    Event(const std::string &label) : label(label) {};
};

/**
 * @brief Unique pointer type alias for Event objects.
 */
using event_ptr = std::unique_ptr<Event>;

/**
 * @brief Type alias for a move-only callback function that accepts an Event.
 *
 * The callback takes ownership of the event through a unique_ptr.
 */
using callback = std::move_only_function<void(std::unique_ptr<Event> event)>;

/**
 * @brief Type alias for a move-only action function with no parameters.
 *
 * This can be used to store arbitrary deferred actions or tasks.
 */
using action = std::move_only_function<void()>;

}
