
#include "xyzlabs/globals.hpp"
#include "xyzlabs/xyzlabs.hpp"

namespace xyzlabs {

XYZLabs &app() {
    return XYZLabs::instance();
}

EventManager &event_manager() {
    return XYZLabs::instance().event_manager();
}

}
