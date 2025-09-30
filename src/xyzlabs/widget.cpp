
#include <spdlog/spdlog.h>
#include <format>

#include "xyzlabs/xyzlabs.hpp"
#include "xyzlabs/widget.hpp"


Widget::Widget(const std::string &title):
    title_(title), id_(XYZLabs::instance().random_generator().random()), open_(true) {
    titleID_ = std::format("{}##{}", title_, id_);
}

void Widget::show(const ImVec2 &size, const ImVec2& position) {}

void Widget::show_toolbar(const ImVec2 &size, const ImVec2& position) {}

void Widget::close() {
    open_ = false;
}
