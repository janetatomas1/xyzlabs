
#include "xyzlabs/xyzlabs.hpp"

int main() {
    XYZLabs &wave = XYZLabs::instance();
    wave.init("XYZLabs");
    return wave.exec();
}
