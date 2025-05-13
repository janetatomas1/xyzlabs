
#include <spdlog/spdlog.h>
#include <format>

#include "xyzlabs.hpp"
#include "widget.hpp"


Widget::Widget(const std::string &title):
    title_(title), id_(XYZLabs::instance().id_generator().get_id()), open_(true), openPtr_(&open_) {
    titleID_ = std::format("{}##{}", title_, id_);
}

void Widget::show(ImVec2 &size, ImVec2& position) {}

void Widget::disable_closing() {
    openPtr_ = nullptr;
}
