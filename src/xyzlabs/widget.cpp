
#include <spdlog/spdlog.h>
#include <format>

#include "xyzlabs/widget.hpp"
#include "xyzlabs/randomgenerator.hpp"
#include "xyzlabs/globals.hpp"
#include "xyzlabs/windowmanager.hpp"

constexpr ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoCollapse;

namespace xyzlabs {

Widget::Widget(const std::string &title):
    title_(title), id_(random_generator().random()) {
}

void Widget::show(const ImVec2 &size, const ImVec2& position) {}

void Widget::display() {
    Window *window = window_manager().get_current_window();
    const ImVec2 size = {
        static_cast<float>(window->width()),
        static_cast<float>(window->height())
    };
    const ImVec2 pos = {0.0f, 0.0f};

    if(ImGui::Begin("##main_window", nullptr, WINDOW_FLAGS)) {
        show(size, pos);
    }
    ImGui::End();
}

const std::string &Widget::title() const {
    return title_;
}

uint64_t Widget::id() const {
    return id_;
}

}
