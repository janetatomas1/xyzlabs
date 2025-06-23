
#include "xyzlabs/burningforest.hpp"
#include "xyzlabs/xyzlabs.hpp"

#include "xyzlabs/task.hpp"

BurningForest::BurningForest(): OpenGLWidget("Burning forest simulation") {
    auto &app = XYZLabs::instance();
    t.draw_triangle();
    t.init();
}

void BurningForest::update() {
    t.render();
}