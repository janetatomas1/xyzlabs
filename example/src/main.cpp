
#include "xyzlabs/xyzlabs.hpp"
#include "burningforest.hpp"

int main() {
    XYZLabs::instance()
    .set_initial_widget<IntroWidget>()
    .init("Burning forest")
    .exec();
}
