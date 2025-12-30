
#include <xyzlabs3d/viewportwidget.hpp>
#include <xyzlabs/xyzlabs.hpp>
#include <xyzlabs/globals.hpp>

using namespace xyzlabs;

int main(int argc, char** argv) {
    auto &app_ = app();
    app_.init<Window, xyzlabs3d::ViewportWidget>("Triangle");
    return app_.exec();
}
