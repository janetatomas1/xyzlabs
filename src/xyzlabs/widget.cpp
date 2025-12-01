
#include <spdlog/spdlog.h>
#include <format>

#include "xyzlabs/widget.hpp"
#include "xyzlabs/randomgenerator.hpp"
#include "xyzlabs/globals.hpp"
#include "xyzlabs/windowmanager.hpp"


namespace xyzlabs {

Widget::Widget(const std::string &title):
    title_(title), id_(random_generator().random()) {
    windowID = fmt::format("{}##{}", title_, id_);
}

void Widget::show(const ImVec2 &size, const ImVec2& position) {
    ImGui::Button("aaaaaaa", size);
}

void Widget::display(const ImVec2 &size, const ImVec2& position) {
    if(ImGui::Begin(windowID.c_str(), nullptr, windowFlags_)) {
        auto [localSize, localPosition] = layout_.compute(size, position);
        ImGui::SetWindowPos(localPosition);
        ImGui::SetWindowSize(localSize);
        show(localSize, localPosition);
    }
    ImGui::End();
}

const std::string &Widget::title() const {
    return title_;
}

uint64_t Widget::id() const {
    return id_;
}

int Widget::get_flags() {
    return windowFlags_;
}

void Widget::set_flags(int flags) {
    windowFlags_ = flags;
}

RelativeLayout &Widget::layout() {
    return layout_;
}

}
