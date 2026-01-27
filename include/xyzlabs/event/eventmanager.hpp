#pragma once

#include <boost/unordered/unordered_flat_map.hpp>
#include <moodycamel/concurrentqueue.h>

#include "xyzlabs/event/event.hpp"

namespace xyzlabs {

/**
 * @brief Manages events, deferred actions, and their callbacks in a thread-safe way.
 *
 * EventManager allows adding events and actions to thread-safe queues, subscribing
 * callbacks to specific event labels, and dispatching events and actions.
 *
 * Key features:
 * - Events are stored in a lock-free queue and dispatched to a single callback per label.
 * - Actions are deferred tasks stored in a lock-free queue and executed in FIFO order.
 * - Only one callback per event label is supported; subscribing again will replace the previous callback.
 * - Uses spdlog to log subscription, unsubscription, and event addition for observability.
 */
class EventManager {
    /// Thread-safe queue storing events to be dispatched
    moodycamel::ConcurrentQueue<event_ptr> events_;

    /// Thread-safe queue storing deferred actions to execute
    moodycamel::ConcurrentQueue<action> actions_;

    /// Map of event labels to their associated callbacks
    boost::unordered::unordered_flat_map<std::string, callback> callbacks_;

public:
    /**
     * @brief Default constructor.
     *
     * Initializes empty event and action queues and an empty callback map.
     */
    EventManager() = default;

    /**
     * @brief Dispatches all queued events and actions.
     *
     * This method:
     * 1. Processes all events currently in the event queue.
     * 2. For each event, invokes the callback associated with its label (if any).
     *    Only one callback per label is supported; other events with the same label
     *    will reuse the same callback.
     * 3. Processes all actions currently in the action queue in FIFO order.
     *
     * @note Exceptions thrown by callbacks or actions are **not caught**.
     *       The caller of dispatch() is responsible for handling any exceptions.
     */
    void dispatch();

    /**
     * @brief Adds an event to the dispatch queue.
     *
     * The event will be processed the next time `dispatch()` is called.
     * The event is moved into the queue, taking ownership.
     *
     * @param event A unique pointer to the Event to add.
     */
    void add_event(event_ptr event);

    /**
     * @brief Adds a deferred action to be executed during dispatch.
     *
     * The action is moved into the action queue and executed in FIFO order
     * during the next `dispatch()`.
     *
     * @param act A move-only function representing the action.
     */
    void add_action(action act);

    /**
     * @brief Subscribes a callback function to a specific event label.
     *
     * Only one callback can be registered per label. If a callback already exists,
     * it will be replaced.
     * Logs the subscription using spdlog.
     *
     * @param label The event label to subscribe to.
     * @param call The move-only callback function to invoke when the event occurs.
     */
    void subscribe(const std::string &label, callback call);

    /**
     * @brief Unsubscribes the callback associated with a specific event label.
     *
     * If no callback exists for the label, this function does nothing.
     * Logs the unsubscription using spdlog.
     *
     * @param label The event label to unsubscribe from.
     */
    void unsubscribe(const std::string &label);
};

} // namespace xyzlabs
