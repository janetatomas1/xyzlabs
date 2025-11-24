
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
    title_(title), id_(random_generator().random()), windowID(fmt::format("{}##{}", title_, id_)) {
}

void Widget::show(const ImVec2 &size, const ImVec2& position) {}

void Widget::display(const ImVec2 &size, const ImVec2& position) {
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(position);
    
    if(ImGui::Begin(windowID.c_str(), nullptr, WINDOW_FLAGS)) {
        show(size, position);
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
