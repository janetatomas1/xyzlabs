
#include "xyzlabs/xyzlabs.hpp"
#include "burningforest.hpp"

int main() {
    auto &app = XYZLabs::instance();
    app.init<IntroWidget>("Burning forest");
    return app.exec();
}
