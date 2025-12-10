
#include "xyzlabs/globals.hpp"
#include "burningforest.hpp"

using namespace xyzlabs;

int main() {
    app().init<Window, BurningForest>("Burning forest");
    return app().exec();
}
