
#include "xyzlabs/globals.hpp"
#include "burningforest.hpp"

int main() {
    app().init<Window, BurningForest>("Burning forest");
    return app().exec();
}
