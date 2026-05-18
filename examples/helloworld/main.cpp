
#include <xyzlabs/xyzlabs.hpp>
#include <xyzlabs/window.hpp>
#include <xyzlabs/ui/widget.hpp>
#include <xyzlabs/ui/button.hpp>
#include <spdlog/spdlog.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

bool save_png(const std::string& filename,
              const std::vector<unsigned char>& rgba,
              int width,
              int height)
{
    return stbi_write_png(filename.c_str(),
                          width,
                          height,
                          4,                // RGBA
                          rgba.data(),
                          width * 4) != 0;  // stride
}

using namespace xyzlabs;

struct HelloWorldWidget: public Widget {
    Button btn;
    HelloWorldWidget(): Widget("Hello world") {
        btn = Button("Click me !");
        btn.layout().set_size_relative({0.15f, 0.1f});
        btn.layout().set_position_relative({0.4f, 0.3f});
    }

    void show(const ImVec2 &size, const ImVec2 &pos) override {
        if(btn(size, pos)) {
            spdlog::info("Button clicked");
            auto a = window()->export_img(0, 0, size.x, size.y);
            save_png("a.png", a, size.x, size.y);
        }
    }
};

int main() {
    auto app = XYZLabs();
    app.init<Window, HelloWorldWidget>("Hello world");
    return app.exec();
}
