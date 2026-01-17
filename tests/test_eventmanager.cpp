
#include <xyzlabs/eventmanager.hpp>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

using namespace xyzlabs;


TEST(EventManagerTest, NoEvents) {
    EventManager manager;

    EXPECT_NO_THROW(manager.dispatch());
}

TEST(EventManagerTest, SingleEventDispatch) {
    EventManager manager;
    size_t count = 0;

    manager.subscribe("A", [&count](std::unique_ptr<Event>) {
        ++count;
    });

    manager.add_event(std::make_unique<Event>("A"));
    manager.dispatch();

    EXPECT_EQ(count, 1);

    manager.dispatch();
    EXPECT_EQ(count, 1);
}

TEST(EventManagerTest, MultipleEventsDispatch) {
    EventManager manager;
    size_t count = 0;

    manager.subscribe("A", [&count](std::unique_ptr<Event>) {
        ++count;
    });

    manager.subscribe("B", [&count](std::unique_ptr<Event>) {
        ++count;
    });

    manager.add_event(std::make_unique<Event>("A"));
    manager.add_event(std::make_unique<Event>("B"));
    manager.dispatch();

    EXPECT_EQ(count, 2);
}

TEST(EventManagerTest, MultipleEventsDispatchOrder) {
    EventManager manager;

    std::vector<std::string> order;

    manager.subscribe("A", [&order](std::unique_ptr<Event>) {
        order.push_back("A");
    });

    manager.subscribe("B", [&order](std::unique_ptr<Event>) {
        order.push_back("B");
    });

    manager.add_event(std::make_unique<Event>("A"));
    manager.add_event(std::make_unique<Event>("B"));
    manager.dispatch();

    EXPECT_EQ(order.size(), 2);
    EXPECT_EQ(order[0], "A");
    EXPECT_EQ(order[1], "B");
}

TEST(EventManagerTest, MultipleCallbacksSameEvent) {
    EventManager manager;

    std::vector<std::string> order;

    manager.subscribe("A", [&order](std::unique_ptr<Event>) {
        order.push_back("A");
    });

    manager.subscribe("B", [&order](std::unique_ptr<Event>) {
        order.push_back("B");
    });

    manager.add_event(std::make_unique<Event>("A"));
    manager.dispatch();

    EXPECT_EQ(order.size(), 1);
    EXPECT_EQ(order[0], "A");
}

TEST(EventManagerTest, MultipleCallbacksSingleEvent) {
    EventManager manager;

    std::vector<std::string> order;

    manager.subscribe("A", [&order](std::unique_ptr<Event>) {
        order.push_back("A");
    });

    manager.subscribe("B", [&order](std::unique_ptr<Event>) {
        order.push_back("B");
    });

    manager.add_event(std::make_unique<Event>("A"));
    manager.dispatch();

    EXPECT_EQ(order.size(), 1);
    EXPECT_EQ(order[0], "A");
}

TEST(EventManagerTest, Unsubscribe) {
    EventManager manager;

    std::vector<std::string> order;

    manager.subscribe("A", [&order](std::unique_ptr<Event>) {
        order.push_back("A");
    });

    manager.unsubscribe("A");
    manager.add_event(std::make_unique<Event>("A"));
    manager.dispatch();

    EXPECT_EQ(order.size(), 0);
}

TEST(EventManagerTest, UnsubscribeNonExisting) {
    EventManager manager;

    manager.unsubscribe("B");
    EXPECT_NO_THROW(
        manager.dispatch();
    );
}

TEST(EventManagerTest, SingleAction) {
    EventManager manager;

    std::vector<std::string> order;
    manager.add_action([&order]() {
        order.push_back("A");
    });

    EXPECT_NO_THROW(
        manager.dispatch();
    );
    EXPECT_EQ(order.size(), 1);
    EXPECT_EQ(order[0], "A");
}

TEST(EventManagerTest, MultipleActions) {
    EventManager manager;

    std::vector<std::string> order;
    manager.add_action([&order]() {
        order.push_back("A");
    });
    manager.add_action([&order]() {
        order.push_back("B");
    });

    EXPECT_NO_THROW(
        manager.dispatch();
    );
    EXPECT_EQ(order.size(), 2);
    EXPECT_EQ(order[0], "A");
    EXPECT_EQ(order[1], "B");
}

TEST(EventManagerTest, EventsAndActions) {
    EventManager manager;

    std::vector<std::string> order;
    manager.add_action([&order]() {
        order.push_back("A");
    });

    manager.subscribe("B", [&order](event_ptr event) {
        order.push_back("B");
    });

    manager.add_event(std::make_unique<Event>("A"));
    manager.add_event(std::make_unique<Event>("B"));
    manager.dispatch();

    EXPECT_EQ(order.size(), 2);
    EXPECT_EQ(order[0], "B");
    EXPECT_EQ(order[1], "A");
}

TEST(EventManagerTest, ActionAddEvent) {
    EventManager manager;

    std::vector<std::string> order;
    manager.subscribe("B", [&order](event_ptr event) {
        order.push_back("B");
    });

    manager.add_action([&order, &manager]() {
        order.push_back("A");
        manager.add_event(std::make_unique<Event>("B"));
    });

    manager.dispatch();

    EXPECT_EQ(order.size(), 1);
    EXPECT_EQ(order[0], "A");

    manager.dispatch();

    EXPECT_EQ(order.size(), 2);
    EXPECT_EQ(order[0], "A");
    EXPECT_EQ(order[1], "B");
}

TEST(EventManagerTest, ActionAddAction) {
    EventManager manager;

    std::vector<std::string> order;

    manager.add_action([&order, &manager]() {
        order.push_back("A");
        manager.add_action([&order]() {
            order.push_back("B");
        });
    });

    manager.dispatch();

    EXPECT_EQ(order.size(), 2);
    EXPECT_EQ(order[0], "A");
    EXPECT_EQ(order[1], "B");
}
