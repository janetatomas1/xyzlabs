
#include "xyzlabs/globals.hpp"
#include "xyzlabs/xyzlabs.hpp"

namespace xyzlabs {

XYZLabs &app() {
    return XYZLabs::instance();
}

RandomGenerator &random_generator() {
    return XYZLabs::instance().random_generator();
}

WindowManager &window_manager() {
    return XYZLabs::instance().window_manager();
}

EventManager &event_manager() {
    return XYZLabs::instance().event_manager();
}

TaskManager &task_manager() {
    return XYZLabs::instance().task_manager();
}

SettingsManager &settings_manager() {
    return XYZLabs::instance().settings_manager();
}

}
