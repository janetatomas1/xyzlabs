
#include "xyzlabs/burningforest.hpp"
#include "xyzlabs/xyzlabs.hpp"

BurningForest::BurningForest(): Widget("Burning forest simulation") {
    auto &app = XYZLabs::instance();
    backend_.create_framebuffer(app.width(), app.height());
}

void BurningForest::show(const ImVec2 &size, const ImVec2 &pos) {
    backend_.rescale_framebuffer(size.x, size.y);
    backend_.bind_framebuffer();
    backend_.unbind_framebuffer();
}

BurningForest::~BurningForest() {
    backend_.destroy();   
}