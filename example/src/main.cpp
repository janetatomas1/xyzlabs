
#include "xyzlabs/xyzlabs.hpp"
#include "burningforest.hpp"

int main() {
    auto &app = XYZLabs::instance();
    app.init<BurningForest>("Burning forest");
    return app.exec();
}
