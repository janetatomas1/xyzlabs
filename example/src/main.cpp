
#include "xyzlabs/xyzlabs.hpp"
#include "burningforest.hpp"

int main() {
    auto &app = XYZLabs::instance();
    app.set_initial_widget<IntroWidget>();
    app.init("Burning forest");
    return app.exec();
}
