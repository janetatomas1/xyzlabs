
#include <spdlog/spdlog.h>
#include <format>

#include "xyzlabs/widget.hpp"
#include "xyzlabs/randomgenerator.hpp"
#include "xyzlabs/xyzlabs.hpp"
#include "xyzlabs/windowmanager.hpp"
#include "xyzlabs/window.hpp"
#include "xyzlabs/eventmanager.hpp"

namespace xyzlabs {

Widget::Widget(const std::string &title, Widget *parent, Window *window):
    title_(title),
    id_(XYZLabs::random_generator().random()),
    parent_(parent),
    window_(window) {
    windowID_ = fmt::format("{}##{}", title_, id_);
}

void Widget::show(const ImVec2 &size, const ImVec2& position) {}

void Widget::display(const ImVec2 &size, const ImVec2& position) {
    if(ImGui::Begin(windowID_.c_str(), nullptr, windowFlags_)) {
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

const std::string &Widget::window_id() const {
    return windowID_;
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

Widget* Widget::parent() {
    return parent_;
}

void Widget::set_parent(Widget *parent) {
    parent_ = parent;
}

Window* Widget::window() {
    if(window_ != nullptr) {
        return window_;
    }

    if(parent_ != nullptr) {
        return parent_->window();
    }

    return nullptr;
}

void Widget::set_window(Window *window) {
    window_ = window;
}

XYZLabs* Widget::app() {
    if(window_ != nullptr) {
        return window_->window_manager()->app();
    }

    if(parent_ != nullptr) {
        return parent_->app();
    }

    return nullptr;
}

}
