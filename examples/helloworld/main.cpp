
#include <xyzlabs/xyzlabs.hpp>
#include <xyzlabs/window.hpp>
#include <xyzlabs/ui/widget.hpp>
#include <xyzlabs/ui/button.hpp>
#include <spdlog/spdlog.h>

using namespace xyzlabs;

struct HelloWorldWidget: public Widget {
    Button btn;
    HelloWorldWidget() {
        btn = Button("Click me !");
        btn.layout().set_size_relative({0.15f, 0.1f});
        btn.layout().set_position_relative({0.4f, 0.3f});
    }

    void show(const ImVec2 &size, const ImVec2 &pos) override {
        if(btn(size, pos)) {
            spdlog::info("Button clicked");
        }
    }
};

int main() {
    auto app = XYZLabs();
    app.init<Window, HelloWorldWidget>("Hello world");
    return app.exec();
}
