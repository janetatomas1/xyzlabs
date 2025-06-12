
#include <spdlog/spdlog.h>
#include <format>

#include "xyzlabs/xyzlabs.hpp"
#include "xyzlabs/widget.hpp"


Widget::Widget(const std::string &title):
    title_(title), id_(XYZLabs::instance().id_generator().get_id()), open_(true), openPtr_(&open_) {
    titleID_ = std::format("{}##{}", title_, id_);
}

void Widget::show(const ImVec2 &size, const ImVec2& position) {}

void Widget::disable_closing() {
    openPtr_ = nullptr;
}
