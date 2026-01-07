
#include "xyzlabs/globals.hpp"
#include "xyzlabs/xyzlabs.hpp"

namespace xyzlabs {

XYZLabs &app() {
    return XYZLabs::instance();
}

EventManager &event_manager() {
    return XYZLabs::instance().event_manager();
}

TaskManager &task_manager() {
    return XYZLabs::instance().task_manager();
}

ObjectRegistry &object_registry() {
    return XYZLabs::instance().object_registry();
}

}
