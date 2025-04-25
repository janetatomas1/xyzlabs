
#include "xyzlabs.hpp"

int main() {
    XYZLabs &wave = XYZLabs::instance();
    return wave.exec();    
}
